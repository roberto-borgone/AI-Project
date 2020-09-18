import { Component } from '@angular/core';
import { FormControl, Validators } from '@angular/forms';
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
import { SettingsVmDialogComponent } from './settings-vm-dialog.component';

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

  ram: FormControl = new FormControl('', [Validators.min(1)])
  disk: FormControl = new FormControl('', [Validators.min(1)])
  vCPU: FormControl = new FormControl('', [Validators.min(1)])
  invalid: boolean
  message: string



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
    this.subscriptions.add(this.vmService.getVM().subscribe(result => this.vms = result))
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

    let dialogRef = this.dialog.open(SettingsVmDialogComponent, {
      width: '400px',
      data: {ram: this.ram, disk: this.disk, vCPU: this.vCPU, invalid: this.invalid, message: this.message}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.ram.valid && result.disk.valid && result.vCPU.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.vmService.updateVM(vm.id, result.ram.value, result.disk.value, result.vCPU.value)
        .subscribe(authResult => {
            
          if(authResult.ok == false){
            // not logged error message display
            this.invalid = true
            this.message = authResult.error.message
            this.openModifyVMDialog(vm)
          }else{

            // created
            this.invalid = false
            this.ram.reset()
            this.disk.reset()
            this.vCPU.reset()
            this.getVM()
          }
        }))
      }else if(!result){
        // dialog closed i remove data
        // created
        this.invalid = false
        this.ram.reset()
        this.disk.reset()
        this.vCPU.reset()
      }
    }));

  }

  openNewVMDialog(){

    let dialogRef = this.dialog.open(SettingsVmDialogComponent, {
      width: '400px',
      data: {ram: this.ram, disk: this.disk, vCPU: this.vCPU, invalid: this.invalid, message: this.message}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.ram.valid && result.disk.valid && result.vCPU.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.vmService.newVM(result.ram.value, result.disk.value, result.vCPU.value)
        .subscribe(authResult => {
            
          if(authResult.ok == false){
            // not logged error message display
            this.invalid = true
            this.message = authResult.error.message
            this.openNewVMDialog()
          }else{

            // created
            this.invalid = false
            this.ram.reset()
            this.disk.reset()
            this.vCPU.reset()
            this.getVM()
          }
        }))
      }else if(!result){
        // dialog closed i remove data
        // created
        this.invalid = false
        this.ram.reset()
        this.disk.reset()
        this.vCPU.reset()
      }
    }));
    
  }

  deleteVM(vm: VM){
    this.vmService.delete(vm.id).subscribe(result => this.getVM())
  }

  startVM(vm: VM){

  }

  stopVM(vm: VM){

  }

}
