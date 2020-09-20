import { Component, OnDestroy } from '@angular/core';
import { TeamService } from '../services/team.service';
import { Subscription } from 'rxjs';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { UpdateVMDialogComponent } from './update-vm-dialog.component';
import { FormControl, Validators } from '@angular/forms';
import { Router } from '@angular/router';
import { CourseService } from '../services/course.service';
import { Course } from '../models/course.model';
import { ModelVM } from '../models/modelVM.model';
import { VmService } from '../services/vm.service';
import { OwnerDialogComponent } from './owner-dialog.component';
import { Team } from '../models/team.model';
import { VM } from '../models/vm.model';
import { Student } from '../models/student.model';

@Component({
  selector: 'app-vms-cont',
  templateUrl: './vms-cont.component.html',
  styleUrls: ['./vms-cont.component.css']
})
export class VmsContComponent implements OnDestroy {

  public dialogInfo: MatDialogRef<DialogInfo>


  subsciptions: Subscription = new Subscription()
  teams: Team[]
  vms: VM[]
  modelVM: ModelVM
  course: Course


  maxRAM: FormControl = new FormControl('', [Validators.min(1)])
  maxDisk: FormControl = new FormControl('', [Validators.min(1)])
  maxVCPU: FormControl = new FormControl('', [Validators.min(1)])
  maxActiveVM: FormControl = new FormControl('', [Validators.min(1)])
  maxTotVM: FormControl = new FormControl('', [Validators.min(1)])
  updateVMInvalid: boolean = false
  message: String = ""

  subscriptions: Subscription = new Subscription()

  constructor(private vmService: VmService, private teamService: TeamService, private dialog: MatDialog, private router: Router, private courseService: CourseService) {
    this.modelVM = new ModelVM('','');
    this.course = new Course('','',0,0,false);
    this.getTeams()
    this.getModelVM()
    this.getCourse()
    this.getVM()
  }

  getTeams(){
    this.subscriptions.add(this.teamService.query().subscribe(result => {this.teams = result}))
  }

  getModelVM() {
    this.subscriptions.add(this.courseService.getModelVM().subscribe(result => { this.modelVM = result }))
  }

  getCourse() {
    this.subscriptions.add(this.courseService.getCourse().subscribe(result => { this.course = result }))
  }

  getVM(){
    this.subscriptions.add(this.vmService.query().subscribe(result => this.vms = result))
  }

  

  openUpdateVMDialog(team: Team){

    let dialogRef = this.dialog.open(UpdateVMDialogComponent, {
      width: '400px',
      data: {maxRAM: this.maxRAM, maxDisk: this.maxDisk, maxVCPU: this.maxVCPU, maxActiveVM: this.maxActiveVM, maxTotVM: this.maxTotVM, updateVMInvalid: this.updateVMInvalid}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.maxRAM.valid && result.maxDisk.valid && result.maxVCPU.valid && result.maxActiveVM.valid && result.maxTotVM.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.teamService.updateVM(new Team(team.id, team.name, result.maxRAM.value, result.maxDisk.value, result.maxVCPU.value, result.maxActiveVM.value, result.maxTotVM.value))
        .subscribe(authResult => {
            
          if(authResult.ok == false){
            // not logged error message display
            this.updateVMInvalid = true
            this.message = authResult.error.message
            this.openUpdateCourseVMDialog()
          }else{

            // created
            this.updateVMInvalid = false
            this.maxRAM.reset()
            this.maxDisk.reset()
            this.maxTotVM.reset()
            this.maxVCPU.reset()
            this.maxActiveVM.reset()
            this.getTeams()
          }
        }))
      }else if(!result){
        // dialog closed i remove data
        // created
        this.updateVMInvalid = false
        this.maxRAM.reset()
        this.maxDisk.reset()
        this.maxTotVM.reset()
        this.maxVCPU.reset()
        this.maxActiveVM.reset()
      }
      
      this.getTeams()
      this.getModelVM()
      this.getCourse()
      this.getVM()

    }));
    
  }

  openUpdateCourseVMDialog(){

    let dialogRef = this.dialog.open(UpdateVMDialogComponent, {
      width: '400px',
      data: {maxRAM: this.maxRAM, maxDisk: this.maxDisk, maxVCPU: this.maxVCPU, maxActiveVM: this.maxActiveVM, maxTotVM: this.maxTotVM, updateVMInvalid: this.updateVMInvalid, message : this.message}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.maxRAM.valid, result.maxDisk.valid, result.maxVCPU.valid, result.maxActiveVM.valid, result.maxTotVM.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.teamService.updateCourseVM(result.maxRAM.value, result.maxDisk.value, result.maxVCPU.value, result.maxActiveVM.value, result.maxTotVM.value)
        .subscribe(authResult => {
          if(authResult.ok == false){
            // not logged error message display
            this.updateVMInvalid = true
            this.message = authResult.error.message
            this.openUpdateCourseVMDialog()
          }else{

            // created
            this.updateVMInvalid = false
            this.maxRAM.reset()
            this.maxDisk.reset()
            this.maxTotVM.reset()
            this.maxVCPU.reset()
            this.maxActiveVM.reset()
            this.getCourse()
          }
        }))
      }else if(!result){
        // dialog closed i remove data
        // created
        this.updateVMInvalid = false
        this.maxRAM.reset()
        this.maxDisk.reset()
        this.maxTotVM.reset()
        this.maxVCPU.reset()
        this.maxActiveVM.reset()
      }
    }));
    
  }

  showInfo(open: boolean){
    
    if(open === true){
      console.log("Aperto")
      this.dialogInfo = this.dialog.open(DialogInfo);
    } else if(open === false){
      this.dialogInfo.close();
    }

  }

  showOwnersDialog(students : Student[]){

    let dialogRef = this.dialog.open(OwnerDialogComponent, {
      width: '400px',
      data: {students: students}
    });
  }

  ngOnDestroy(){
    this.subscriptions.unsubscribe()
  }

}

@Component({
  selector: 'info-dialog',
  templateUrl: 'infoDialog.html',
})
export class DialogInfo {}
