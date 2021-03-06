import { Component, ViewChild, Input, Output, EventEmitter } from '@angular/core';
import { AuthService } from '../auth/auth.service';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { SelectionModel } from '@angular/cdk/collections';
import { Student } from '../models/student.model';
import { Proposal } from '../models/proposal.model';

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

  @ViewChild('sort4', {read: MatSort, static: true})
  sort4: MatSort

  @ViewChild('pag4', {read: MatPaginator}) 
  paginator4: MatPaginator

  myTeam: MatTableDataSource<Student>
  enrolledStudents: MatTableDataSource<Student>
  studentInProposal: MatTableDataSource<Student>

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

  @Input()set _studentInProposal(inProposal: Student[]){
    this.studentInProposal = new MatTableDataSource(inProposal)
    this.studentInProposal.sort = this.sort3
    this.studentInProposal.paginator = this.paginator3
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
    this.teamProposal.sort = this.sort4
    this.teamProposal.paginator = this.paginator4
  }



  @Output()
  onAccept: EventEmitter<any>
  @Output()
  onReject: EventEmitter<any>

  colsToDisplay: string[] = ['id', 'name', 'surname', 'email']
  colsToDisplay2: string[] = ['select', 'id', 'name', 'surname']
  colsToDisplayProposal: string[] = ['nomegruppo', 'creator','members','button']
  colsToDisplaySospeso: string[] = ['button_p','id_p','name_p','surname_p']
  
  
  constructor(public auth: AuthService) {
    this.selection = new SelectionModel<Student>(true, [])
    this.onTeamUp = new EventEmitter()
    this.onAccept = new EventEmitter()
    this.onReject = new EventEmitter()
  }

  teamUpSelected(){
    this.onTeamUp.emit(this.selection.selected)
    this.selection = new SelectionModel<Student>(true, [])
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
