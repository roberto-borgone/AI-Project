import { Component, ViewChild, Input, Output, EventEmitter } from '@angular/core';
import { AuthService } from '../auth/auth.service';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { Student } from '../student.model';
import { MatTableDataSource } from '@angular/material/table';
import { SelectionModel } from '@angular/cdk/collections';

@Component({
  selector: 'app-team',
  templateUrl: './team.component.html',
  styleUrls: ['./team.component.css']
})
export class TeamComponent {

  @ViewChild('sort1', {read: MatSort, static: true})
  sort: MatSort

  @ViewChild('pag1', {read: MatPaginator}) 
  paginator: MatPaginator

  @ViewChild('sort2', {read: MatSort, static: true})
  sort2: MatSort

  @ViewChild('pag2', {read: MatPaginator}) 
  paginator2: MatPaginator

  myTeam: MatTableDataSource<Student>
  enrolledStudents: MatTableDataSource<Student>

  @Output()
  onTeamUp: EventEmitter<Student[]>

  selection: SelectionModel<Student>

  @Input()
  set _team(team: Student[]){
    this.myTeam = new MatTableDataSource(team)
    this.myTeam.sort = this.sort
    this.myTeam.paginator = this.paginator
  }

  @Input()
  set _enrolledStudents(students: Student[]){
    this.enrolledStudents = new MatTableDataSource(students)
    this.enrolledStudents.sort = this.sort2
    this.enrolledStudents.paginator = this.paginator2
  }

  colsToDisplay: string[] = ['id', 'name', 'surname', 'email']
  colsToDisplay2: string[] = ['select', 'id', 'name', 'surname']
  
  constructor(public auth: AuthService) {
    this.selection = new SelectionModel<Student>(true, [])
    this.onTeamUp = new EventEmitter()
  }

  teamUpSelected(){
    if(this.selection.selected.length != 0){
      this.onTeamUp.emit(this.selection.selected)
      this.selection = new SelectionModel<Student>(true, [])
    }
  }

  toggleSelectionRow(event: Event, student: Student) {
    this.selection.toggle(student)
  }

  isRowSelected(student: Student): boolean {
    return this.selection.isSelected(student)
  }

}
