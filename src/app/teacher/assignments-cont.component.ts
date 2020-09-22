import { Component, OnDestroy, OnInit } from '@angular/core';
import { FormControl, Validators } from '@angular/forms';
import { MatDialog, MatDialogConfig, MatDialogRef } from '@angular/material/dialog';
import { Subscription } from 'rxjs';
import { Assignment } from '../models/assignment.model';
import { ContentDialogComponent } from '../content-dialog.component';
import { AssignmentService } from '../services/assignment.service';
import { CourseService } from '../services/course.service';
import { NewAssignmentDialogComponent } from './new-assignment-dialog.component';
import { WorkDialogComponent } from './work-dialog.component';
import { NavigationEnd, Router } from '@angular/router';

@Component({
  selector: 'app-assignments-cont',
  templateUrl: './assignments-cont.component.html',
  styleUrls: ['./assignments-cont.component.css']
})
export class AssignmentsContComponent implements OnDestroy {

  assignments: Assignment[]
  subscriptions: Subscription = new Subscription()
  img: any;

  dueDate: FormControl = new FormControl('', Validators.required)
  file: any

  constructor(private router: Router, private courseService : CourseService, private assignmentService : AssignmentService, private dialog: MatDialog) {
    this.subscriptions.add(this.router.events.subscribe(val => {
      if(val instanceof NavigationEnd){
        this.getAssignments();
      }
    }))
  }

  getAssignments() {
    this.subscriptions.add(this.assignmentService.getAssignmentsDocent().subscribe(assignments => {this.assignments = assignments}));
  }

  openContentDialog(assignment: Assignment){
    console.log("Open content dialog");
    this.subscriptions.add(this.assignmentService.getAssignmentContent(assignment).subscribe(img => {

      const dialogConfig = new MatDialogConfig();
      dialogConfig.width = '80%';
      dialogConfig.height = '70%';
      dialogConfig.data = img;
  
      let dialogRef = this.dialog.open(ContentDialogComponent, dialogConfig);
  
      this.subscriptions.add(dialogRef.afterClosed().subscribe());
    }));
  }

  openStudentsDialog(assignmentId : number){

    this.subscriptions.add(this.assignmentService.getStudentsWorks(assignmentId).subscribe(works => {
      console.log(works); 
      const dialogConfig = new MatDialogConfig();
      dialogConfig.width = '98%';
      dialogConfig.height = '70%';
      dialogConfig.data = works;
  
      let dialogRef = this.dialog.open(WorkDialogComponent, dialogConfig);
  
      this.subscriptions.add(dialogRef.afterClosed().subscribe());
    }));
  }

  openAssignmentDialog(error: string) {
    const dialogConfig = new MatDialogConfig();
    dialogConfig.width = '35%';
    dialogConfig.height = '40%';
    dialogConfig.data = {dueDate: this.dueDate, file: this.file, error: error};

    let dialogRef = this.dialog.open(NewAssignmentDialogComponent, dialogConfig);

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result!=undefined) {
        if(result.dueDate!=undefined && result.file!=undefined) {
          let currentDateTime: Date = new Date();
          console.log(currentDateTime);
          console.log(new Date(result.dueDate.value));
          if(new Date(result.dueDate.value) <= currentDateTime) {
            let err: string = "Inserire una data di scadenza valida";
            this.openAssignmentDialog(err);
          }
          else {
            this.subscriptions.add(this.assignmentService.createAssignment(result.dueDate.value, result.file).subscribe(res => {if(res) this.getAssignments()}));
          }
        }
        else {
          let err: string = "Inserire una data di scadenza valida e caricare il testo della consegna";
          this.openAssignmentDialog(err);    
        }
      }
    }));
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe();
  }
}
