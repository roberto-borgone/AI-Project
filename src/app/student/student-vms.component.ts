import { Component, EventEmitter, Input, Output, ViewChild } from '@angular/core';
import { MatPaginator } from '@angular/material/paginator';
import { MatSort } from '@angular/material/sort';
import { MatTableDataSource } from '@angular/material/table';
import { AuthService } from '../auth/auth.service';
import { Course } from '../models/course.model';
import { Student } from '../models/student.model';
import { VM } from '../models/vm.model';
import { ModelVM } from '../models/modelVM.model';

@Component({
  selector: 'app-student-vms',
  templateUrl: './student-vms.component.html',
  styleUrls: ['./student-vms.component.css']
})
export class StudentVmsComponent {

  @ViewChild(MatSort, {static: true})
  sort: MatSort

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  @Output()
  onNewVM: EventEmitter<any>

  @Output()
  onShowOwners: EventEmitter<Student[]>

  @Output()
  onModifyVM: EventEmitter<VM>

  @Output()
  onDeleteVM: EventEmitter<VM>

  @Output()
  onStartVM: EventEmitter<VM>

  @Output()
  onStopVM: EventEmitter<VM>

  colsToDisplay: string[] = ['on', 'id', 'ram', 'virtualCpu', 'disk', 'teamID', 'owners', 'commands']

  vms: MatTableDataSource<VM>
  modelVM: ModelVM
  course: Course

  @Input()
  set _vms(vms: VM[]){
    this.vms = new MatTableDataSource(vms)
    this.vms.sort = this.sort
    this.vms.paginator = this.paginator
  }

  @Input()
  set _modelVM(modelVM: ModelVM) {
    this.modelVM = modelVM
  }

  @Input()
  set _course(course: Course) {
    this.course = course
  }

  constructor(private auth: AuthService) { 
    this.onNewVM = new EventEmitter()
    this.onDeleteVM = new EventEmitter()
    this.onModifyVM = new EventEmitter()
    this.onShowOwners = new EventEmitter()
    this.onStartVM = new EventEmitter()
    this.onStopVM = new EventEmitter()
  }

  showOwners(students: Student[]){
    this.onShowOwners.emit(students)
  }

  newVM(){
    this.onNewVM.emit()
  }

  modifyVM(vm: VM){
    this.onModifyVM.emit(vm)
  }

  deleteVM(vm: VM){
    this.onDeleteVM.emit(vm)
  }

  startVM(vm: VM){
    this.onStartVM.emit(vm)
  }

  stopVM(vm: VM){
    this.onStopVM.emit(vm)
  }

  isOwner(element: VM){
    return element.owners.find(s => s.id == this.auth.token.username) != undefined
  }

}
