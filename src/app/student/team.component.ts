import { Component, OnInit, ViewChild, Input } from '@angular/core';
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

  @Input()
  set _team(team: Student[]){
    this.myTeam = new MatTableDataSource(team)
    this.myTeam.sort = this.sort
    this.myTeam.paginator = this.paginator
  }

  colsToDisplay: string[] = ['id', 'name', 'surname', 'email']
  
  constructor(public auth: AuthService) { }

  ngOnInit(): void {
  }

}
