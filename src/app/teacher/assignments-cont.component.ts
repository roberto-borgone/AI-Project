import { Component, OnDestroy, OnInit } from '@angular/core';
import { Subscription } from 'rxjs';
import { Assignment } from '../assignment.model';
import { CourseService } from '../services/course.service';

@Component({
  selector: 'app-assignments-cont',
  templateUrl: './assignments-cont.component.html',
  styleUrls: ['./assignments-cont.component.css']
})
export class AssignmentsContComponent implements OnDestroy {

  assignments: Assignment[]
  subscriptions: Subscription = new Subscription()

  constructor(private courseService : CourseService) {
    this.getAssignments();
  }

  getAssignments() {
    this.subscriptions.add(this.courseService.getAssignmentsService().subscribe(assignments => this.assignments = assignments));
  }

  openContentDialog(assignmentId : number){
    console.log("Open content dialog");
  }

  openStudentsDialog(assignmentId : number){
    this.subscriptions.add(this.courseService.getAssignmentsService().subscribe(assignments => this.assignments = assignments));
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe();
  }
}
