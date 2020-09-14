import { TeamService } from './../services/team.service';
import { Proposal } from './../proposal.model';
import { Component, OnInit, ViewChild, Input, Output, EventEmitter } from '@angular/core';
import { AuthService } from '../auth/auth.service';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { Student } from '../student.model';
import { MatTableDataSource } from '@angular/material/table';

@Component({
  selector: 'app-team',
  templateUrl: './team.component.html',
  styleUrls: ['./team.component.css']
})
export class TeamComponent implements OnInit {

  

  @ViewChild('sort1', {read: MatSort, static: true})
  sort: MatSort

  @ViewChild('pag1', {read: MatPaginator}) 
  paginator: MatPaginator

  @ViewChild('sort2', {read: MatSort, static: true})
  sort2: MatSort

  @ViewChild('pag2', {read: MatPaginator}) 
  paginator2: MatPaginator

  myTeam: MatTableDataSource<Student>
  teamProposal: MatTableDataSource<Proposal>

  @Input()
  set _team(team: Student[]){
    this.myTeam = new MatTableDataSource(team)
    this.myTeam.sort = this.sort
    this.myTeam.paginator = this.paginator
  }

  @Input()
  set _proposal(propose: Proposal[]){
    this.teamProposal = new MatTableDataSource(propose)
    this.teamProposal.sort = this.sort
    this.teamProposal.paginator = this.paginator
  }

  @Output()
  onAccept: EventEmitter<any>
  @Output()
  onReject: EventEmitter<any>

  arrayOne(n: number): any[] {
    return Array(n);
  }


  colsToDisplay: string[] = ['id', 'name', 'surname', 'email']

  colsToDisplayProposal: string[] = ['nomegruppo', 'creator','members','button']

  colsToDisplaySospeso: string[] = ['nomegruppo','members']
  
  constructor(
    public auth: AuthService) {
      this.onAccept = new EventEmitter()
      this.onReject = new EventEmitter()
     }

  accept(id) { 
    this.onAccept.emit(id);
  }
  reject(id) { 
    this.onReject.emit(id);
  }

  ngOnInit(): void {
  }

}
