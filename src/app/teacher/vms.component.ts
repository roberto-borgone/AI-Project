import { Component, ViewChild, Input, Output, EventEmitter, QueryList } from '@angular/core';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { Team } from '../team.model';
import { CourseService } from '../services/course.service';
import { VM } from '../vm.model';
import { Student } from '../student.model';
import { throwIfEmpty } from 'rxjs/operators';
import { ModelVM } from './modelVM.model';
import { Course } from '../course.model';

@Component({
  selector: 'app-vms',
  templateUrl: './vms.component.html',
  styleUrls: ['./vms.component.css']
})
export class VmsComponent {

  @ViewChild('sort1', {read: MatSort, static: true})
  sort: MatSort

  @ViewChild('pag1', {read: MatPaginator}) 
  paginator: MatPaginator

  @ViewChild('sort2', {read: MatSort, static: true})
  sort2: MatSort

  @ViewChild('pag2', {read: MatPaginator}) 
  paginator2: MatPaginator

  @Output()
  onUpdateVM: EventEmitter<Team>

  @Output()
  onShowOwners: EventEmitter<Student[]>
  onUpdateCourseVM: EventEmitter<any>

  colsToDisplay: string[] = ['id', 'name', 'maxRAM', 'maxDisk', 'maxVCPU', 'maxActiveVM', 'maxTotVM', 'buttons']
  colsToDisplayVM: string[] = ['isOn', 'id', 'ram', 'virtualCpu', 'disk', 'teamID', 'owners', 'path']

  teams: MatTableDataSource<Team>
  vms: MatTableDataSource<VM>

  modelVM: ModelVM

  course: Course

  @Input()
  set _teams(teams: Team[]){
    this.teams = new MatTableDataSource(teams)
    this.teams.sort = this.sort
    this.teams.paginator = this.paginator
  }

  @Input()
  set _vms(vms: VM[]){
    this.vms = new MatTableDataSource(vms)
    this.vms.sort = this.sort2
    this.vms.paginator = this.paginator2
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
    this.onShowOwners = new EventEmitter()
    this.onUpdateCourseVM = new EventEmitter()
  }

  updateVM(team: Team){
    this.onUpdateVM.emit(team)
  }

  showOwners(students: Student[]){
    this.onShowOwners.emit(students)
  }

  startVM(vm: VM){
    console.log("VM " + vm.id + " has started")
  }
  
  updateCourseVM() {
    this.onUpdateCourseVM.emit();
  }

}
