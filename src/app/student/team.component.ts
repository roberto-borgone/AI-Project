import { Component, ViewChild, Input, Output, EventEmitter } from '@angular/core';
import { Proposal } from './../proposal.model';
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

  @ViewChild('sort3', {read: MatSort, static: true})
  sort3: MatSort

  @ViewChild('pag3', {read: MatPaginator}) 
  paginator3: MatPaginator

  myTeam: MatTableDataSource<Student>
  enrolledStudents: MatTableDataSource<Student>

  @Output()
  onTeamUp: EventEmitter<Student[]>

  selection: SelectionModel<Student>
  teamProposal: MatTableDataSource<Proposal>

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

  @Input()
  set _proposal(propose: Proposal[]){
    this.teamProposal = new MatTableDataSource(propose)
    this.teamProposal.sort = this.sort3
    this.teamProposal.paginator = this.paginator3
  }

  @Output()
  onAccept: EventEmitter<any>
  @Output()
  onReject: EventEmitter<any>

  colsToDisplay: string[] = ['id', 'name', 'surname', 'email']
  colsToDisplay2: string[] = ['select', 'id', 'name', 'surname']
  colsToDisplayProposal: string[] = ['nomegruppo', 'creator','members','button']
  colsToDisplaySospeso: string[] = ['nomegruppo','members']
  
  
  constructor(public auth: AuthService) {
    this.selection = new SelectionModel<Student>(true, [])
    this.onTeamUp = new EventEmitter()
    this.onAccept = new EventEmitter()
    this.onReject = new EventEmitter()
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

  arrayOne(n: number): any[] {
    return Array(n);
  }

  accept(id) { 
    this.onAccept.emit(id);
  }
  reject(id) { 
    this.onReject.emit(id);
  }

  isRowSelected(student: Student): boolean {
    return this.selection.isSelected(student)
  }

}
