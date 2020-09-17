import { Component, EventEmitter, Input, OnInit, Output, ViewChild } from '@angular/core';
import { MatPaginator } from '@angular/material/paginator';
import { MatSort } from '@angular/material/sort';
import { MatTableDataSource } from '@angular/material/table';
import { Assignment } from '../models/assignment.model';

@Component({
  selector: 'app-assignments',
  templateUrl: './assignments.component.html',
  styleUrls: ['./assignments.component.css']
})
export class AssignmentsComponent implements OnInit {

  @ViewChild(MatSort, {static: true})
  sort: MatSort

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  // column to display in table
  colsToDisplay: string[] = ['id', 'releaseDate', 'dueDate', 'content', 'students']

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
  onShowStudents: EventEmitter<any>;

  @Output()
  onCreateAssignment: EventEmitter<any>;

  constructor() { 
    this.onShowContent = new EventEmitter<any>();
    this.onShowStudents = new EventEmitter<any>();
    this.onCreateAssignment = new EventEmitter();
  }

  showContent(assignmentId : number){
    this.onShowContent.emit(assignmentId);
  }

  showStudents(assignmentId : number){
    this.onShowStudents.emit(assignmentId);
  }

  createAssignment() {
    this.onCreateAssignment.emit();
  }

  ngOnInit(): void {
  }

}
