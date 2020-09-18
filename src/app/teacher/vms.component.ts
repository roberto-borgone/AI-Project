import { Component, ViewChild, Input, Output, EventEmitter, QueryList, ViewChildren } from '@angular/core';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { CourseService } from '../services/course.service';

import { Overlay, OverlayConfig, OverlayRef } from '@angular/cdk/overlay';
import { Portal, TemplatePortalDirective } from '@angular/cdk/portal';
import { ModelVM } from '../models/modelVM.model';
import { Course } from '../models/course.model';
import { Team } from '../models/team.model';
import { Student } from '../models/student.model';
import { VM } from '../models/vm.model';

@Component({
  selector: 'app-vms',
  templateUrl: './vms.component.html',
  styleUrls: ['./vms.component.css']
})
export class VmsComponent {


  isMenuOpen: boolean = false;

  fusilliOverlayRef: OverlayRef;

  @ViewChildren(TemplatePortalDirective) templatePortals: QueryList<Portal<any>>;

  
  open: boolean
  isOpen = false;

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

  @Output()
  onShowInfo: EventEmitter<boolean>

  @Output()
  onUpdateCourseVM: EventEmitter<any>

  colsToDisplay: string[] = ['id', 'name', 'maxRAM', 'maxDisk', 'maxVCPU', 'maxActiveVM', 'maxTotVM', 'buttons']
  colsToDisplayVM: string[] = ['on', 'id', 'ram', 'virtualCpu', 'disk', 'teamID', 'owners', 'path']

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


  constructor(public courseService: CourseService, public overlay: Overlay) { 
    this.onUpdateVM = new EventEmitter()
    this.onShowOwners = new EventEmitter<Student[]>()
    this.onUpdateCourseVM = new EventEmitter()
    this.onShowInfo = new EventEmitter()
    this.open = false
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

  enter() {
    let config = new OverlayConfig();

    config.positionStrategy = this.overlay.position()
        .global()
        .centerHorizontally()
        .top(`10%`);


    this.fusilliOverlayRef = this.overlay.create(config);
    this.fusilliOverlayRef.attach(this.templatePortals.first);
  }

  leave() {
    this.fusilliOverlayRef.dispose();
  }
}
