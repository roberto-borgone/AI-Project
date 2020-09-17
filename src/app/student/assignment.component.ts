import { Component, EventEmitter, Input, OnInit, Output, ViewChild } from '@angular/core';
import { MatPaginator } from '@angular/material/paginator';
import { MatSort } from '@angular/material/sort';
import { MatTableDataSource } from '@angular/material/table';
import { Assignment } from '../assignment.model';

@Component({
  selector: 'app-assignment',
  templateUrl: './assignment.component.html',
  styleUrls: ['./assignment.component.css']
})
export class AssignmentComponent implements OnInit {

  @ViewChild(MatSort, {static: true})
  sort: MatSort

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  // column to display in table
  colsToDisplay: string[] = ['id', 'releaseDate', 'dueDate', 'content', 'works']

  assignments: MatTableDataSource<Assignment>

  @Input()
  set _assignments(assignments: Assignment[]){
    this.assignments = new MatTableDataSource(assignments)
    this.assignments.sort = this.sort
    this.assignments.paginator = this.paginator
  }

  @Output()
  onShowContent: EventEmitter<any>;

  @Output()
  onShowWorks: EventEmitter<any>;

  constructor() { 
    this.onShowContent = new EventEmitter<any>();
    this.onShowWorks = new EventEmitter<any>();
  }

  showContent(assignmentId : number){
    this.onShowContent.emit(assignmentId);
  }

  showWorks(assignment: Assignment){
    this.onShowWorks.emit(assignment);
  }

  ngOnInit(): void {
  }

}
