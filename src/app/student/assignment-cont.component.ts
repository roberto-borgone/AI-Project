import { Component, OnDestroy, OnInit } from '@angular/core';
import { FormControl, Validators } from '@angular/forms';
import { MatDialog, MatDialogConfig } from '@angular/material/dialog';
import { Subscription } from 'rxjs';
import { Assignment } from '../assignment.model';
import { ContentDialogComponent } from '../content-dialog.component';
import { AssignmentService } from '../services/assignment.service';
import { CourseService } from '../services/course.service';
import { WorksDialogComponent } from './works-dialog.component';

@Component({
  selector: 'app-assignment-cont',
  templateUrl: './assignment-cont.component.html',
  styleUrls: ['./assignment-cont.component.css']
})
export class AssignmentContComponent implements OnDestroy {

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

  openWorksDialog(assignmentId : number){

    this.subscriptions.add(this.assignmentService.getStudentsWorks(assignmentId).subscribe(works => {
      console.log(works); 
      const dialogConfig = new MatDialogConfig();
      dialogConfig.width = '98%';
      dialogConfig.height = '70%';
      dialogConfig.data = works;
  
      let dialogRef = this.dialog.open(WorksDialogComponent, dialogConfig);
  
      this.subscriptions.add(dialogRef.afterClosed().subscribe());
    }));
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe();
  }
}
