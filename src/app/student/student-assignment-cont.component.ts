import { Component, OnDestroy, OnInit } from '@angular/core';
import { FormControl, Validators } from '@angular/forms';
import { MatDialog, MatDialogConfig } from '@angular/material/dialog';
import { NavigationEnd, Router } from '@angular/router';
import { Subscription } from 'rxjs';
import { ContentDialogComponent } from '../content-dialog.component';
import { Assignment } from '../models/assignment.model';
import { LastWork } from '../models/last-work.model';
import { AssignmentService } from '../services/assignment.service';
import { WorksDialogComponent } from './works-dialog.component';

@Component({
  selector: 'app-student-assignment-cont',
  templateUrl: './student-assignment-cont.component.html',
  styleUrls: ['./student-assignment-cont.component.css']
})
export class StudentAssignmentContComponent implements OnDestroy {

  assignments: Assignment[]
  subscriptions: Subscription = new Subscription()
  img: any;

  dueDate: FormControl = new FormControl('', Validators.required)
  file: any

  constructor(private router: Router, private assignmentService : AssignmentService, private dialog: MatDialog) {
    this.subscriptions.add(this.router.events.subscribe(val => {
      if(val instanceof NavigationEnd){
        this.getAssignments();
      }
    }))
  }

  getAssignments() {
    this.subscriptions.add(this.assignmentService.getAssignmentsStudent().subscribe(assignments => {this.assignments = assignments}));
  }

  openContentDialog(assignment: Assignment){
    console.log("Open content dialog");
    this.subscriptions.add(this.assignmentService.getAssignmentContent(assignment).subscribe(img => {

      const dialogConfig = new MatDialogConfig();
      dialogConfig.width = '80%';
      dialogConfig.height = '70%';
      dialogConfig.data = img;
  
      let dialogRef = this.dialog.open(ContentDialogComponent, dialogConfig);
  
      this.subscriptions.add(dialogRef.afterClosed().subscribe(res => this.getAssignments()));
    }));
  }

  openWorksDialog(assignment: Assignment){

    this.subscriptions.add(this.assignmentService.getStudentWorksStudent(assignment.id).subscribe(worksData => {

      this.subscriptions.add(this.assignmentService.getStudentStatus(assignment.id).subscribe(res => {

        let lastWorkData: LastWork[] = []
        lastWorkData.push(res);

        const dialogConfig = new MatDialogConfig();
        dialogConfig.width = '98%';
        dialogConfig.height = '70%';
        dialogConfig.data = {
          worksData: worksData,
          lastWorkData: lastWorkData
        }
    
        let dialogRef = this.dialog.open(WorksDialogComponent, dialogConfig);
    
        this.subscriptions.add(dialogRef.afterClosed().subscribe(res => this.getAssignments()));
      }))
    }));
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe();
  }

}
