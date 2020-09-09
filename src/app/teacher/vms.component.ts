import { Component, ViewChild, Input, Output, EventEmitter } from '@angular/core';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { Team } from '../team.model';
import { CourseService } from '../services/course.service';

@Component({
  selector: 'app-vms',
  templateUrl: './vms.component.html',
  styleUrls: ['./vms.component.css']
})
export class VmsComponent {

  @ViewChild(MatSort, {static: true})
  sort: MatSort

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  @Output()
  onUpdateVM: EventEmitter<Team>

  colsToDisplay: string[] = ['id', 'name', 'maxRAM', 'maxDisk', 'maxVCPU', 'maxActiveVM', 'maxTotVM', 'buttons']

  teams: MatTableDataSource<Team>

  @Input()
  set _teams(teams: Team[]){
    this.teams = new MatTableDataSource(teams)
    this.teams.sort = this.sort
    this.teams.paginator = this.paginator
  }

  constructor(public courseService: CourseService) { 
    this.onUpdateVM = new EventEmitter()
  }

  updateVM(team: Team){
    this.onUpdateVM.emit(team)
  }

}
