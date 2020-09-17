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

  constructor(private courseService : CourseService, private assignmentService : AssignmentService, private dialog: MatDialog) {
    this.getAssignments();
  }

  getAssignments() {
    this.subscriptions.add(this.courseService.getAssignmentsService().subscribe(assignments => {console.log(assignments); this.assignments = assignments}));
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

  openAssignmentDialog() {
    const dialogConfig = new MatDialogConfig();
    dialogConfig.width = '40%';
    dialogConfig.height = '40%';
    dialogConfig.data = {dueDate: this.dueDate, file: this.file};

    let dialogRef = this.dialog.open(NewAssignmentDialogComponent, dialogConfig);

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      console.log(result.file)
      this.subscriptions.add(this.assignmentService.createAssignment(result.dueDate.value, result.file).subscribe())
      this.getAssignments()
    }));
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe();
  }
}
