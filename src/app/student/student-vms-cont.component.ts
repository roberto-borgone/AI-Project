import { Component } from '@angular/core';
import { MatDialog } from '@angular/material/dialog';
import { Router } from '@angular/router';
import { Subscription } from 'rxjs';
import { Course } from '../course.model';
import { CourseService } from '../services/course.service';
import { VmService } from '../services/vm.service';
import { Student } from '../student.model';
import { ModelVM } from '../teacher/modelVM.model';
import { OwnerDialogComponent } from '../teacher/owner-dialog.component';
import { VM } from '../vm.model';

@Component({
  selector: 'app-student-vms-cont',
  templateUrl: './student-vms-cont.component.html',
  styleUrls: ['./student-vms-cont.component.css']
})
export class StudentVmsContComponent {

  subscriptions: Subscription = new Subscription()
  vms: VM[]
  modelVM: ModelVM
  course: Course

  constructor(private vmService: VmService, private dialog: MatDialog, private router: Router, private courseService: CourseService) { 
    this.modelVM = new ModelVM('','');
    this.course = new Course('','',0,0,false);
    this.getModelVM()
    this.getVM()
    this.getCourse()
  }

  getModelVM() {
    this.subscriptions.add(this.courseService.getModelVM().subscribe(result => { this.modelVM = result }))
  }

  getVM(){
    this.subscriptions.add(this.vmService.query().subscribe(result => this.vms = result))
  }

  getCourse() {
    this.subscriptions.add(this.courseService.getCourse().subscribe(result => { this.course = result }))
  }

  showOwnersDialog(students : Student[]){

    let dialogRef = this.dialog.open(OwnerDialogComponent, {
      width: '400px',
      data: {students: students}
    });
  }

  openModifyVMDialog(vm: VM){

  }

  openNewVMDialog(){
    
  }

  deleteVM(vm: VM){

  }

  startVM(vm: VM){

  }

  stopVM(vm: VM){

  }

}
