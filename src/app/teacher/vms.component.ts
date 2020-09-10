import { Component, ViewChild, Input, Output, EventEmitter } from '@angular/core';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { Team } from '../team.model';
import { CourseService } from '../services/course.service';
import { throwIfEmpty } from 'rxjs/operators';
import { ModelVM } from './modelVM.model';
import { Course } from '../course.model';

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

  @Output()
  onUpdateCourseVM: EventEmitter<any>

  colsToDisplay: string[] = ['id', 'name', 'maxRAM', 'maxDisk', 'maxVCPU', 'maxActiveVM', 'maxTotVM', 'buttons']

  teams: MatTableDataSource<Team>

  modelVM: ModelVM

  course: Course

  @Input()
  set _teams(teams: Team[]){
    this.teams = new MatTableDataSource(teams)
    this.teams.sort = this.sort
    this.teams.paginator = this.paginator
  }

  @Input()
  set _modelVM(modelVM: ModelVM) {
    this.modelVM = modelVM
  }

  @Input()
  set _course(course: Course) {
    this.course = course
  }

  constructor(public courseService: CourseService) { 
    this.onUpdateVM = new EventEmitter()
    this.onUpdateCourseVM = new EventEmitter()
  }

  updateVM(team: Team){
    this.onUpdateVM.emit(team)
  }

  updateCourseVM() {
    this.onUpdateCourseVM.emit();
  }

}
