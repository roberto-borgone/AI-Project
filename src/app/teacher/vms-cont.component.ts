import { Component, OnDestroy } from '@angular/core';
import { TeamService } from '../services/team.service';
import { Subscription } from 'rxjs';
import { Team } from '../team.model';
import { MatDialog } from '@angular/material/dialog';
import { UpdateVMDialogComponent } from './update-vm-dialog.component';
import { FormControl, Validators } from '@angular/forms';
import { Router } from '@angular/router';
import { CourseService } from '../services/course.service';
import { VM } from '../vm.model';
import { Course } from '../course.model';
import { ModelVM } from './modelVM.model';

@Component({
  selector: 'app-vms-cont',
  templateUrl: './vms-cont.component.html',
  styleUrls: ['./vms-cont.component.css']
})
export class VmsContComponent implements OnDestroy {

  subsciptions: Subscription = new Subscription()
  teams: Team[]
  vms: VM[] = [
    {id: 1, ram: 2, disk: 200, virtualCpu: 1, isOn: true, path: 'ciao', teamID: 1, owners: []},
    {id: 2, ram: 2, disk: 200, virtualCpu: 1, isOn: true, path: 'ciao', teamID: 1, owners: []},
  ]
  modelVM: ModelVM
  course: Course

  maxRAM: FormControl = new FormControl('', [Validators.min(1)])
  maxDisk: FormControl = new FormControl('', [Validators.min(1)])
  maxVCPU: FormControl = new FormControl('', [Validators.min(1)])
  maxActiveVM: FormControl = new FormControl('', [Validators.min(1)])
  maxTotVM: FormControl = new FormControl('', [Validators.min(1)])
  updateVMInvalid: boolean = false

  subscriptions: Subscription = new Subscription()

  constructor(private teamService: TeamService, private dialog: MatDialog, private router: Router, private courseService: CourseService) {
    this.modelVM = new ModelVM('','');
    this.course = new Course('','',0,0,false);
    this.getTeams()
    this.getModelVM()
    this.getCourse()
  }

  getTeams(){
    this.subsciptions.add(this.teamService.query().subscribe(result => {this.teams = result}))
  }

  getModelVM() {
    this.subsciptions.add(this.courseService.getModelVM().subscribe(result => { this.modelVM = result }))
  }

  getCourse() {
    this.subsciptions.add(this.courseService.getCourse().subscribe(result => { this.course = result }))
  }

  openUpdateVMDialog(team: Team){

    let dialogRef = this.dialog.open(UpdateVMDialogComponent, {
      width: '400px',
      data: {maxRAM: this.maxRAM, maxDisk: this.maxDisk, maxVCPU: this.maxVCPU, maxActiveVM: this.maxActiveVM, maxTotVM: this.maxTotVM, updateVMInvalid: this.updateVMInvalid}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.maxRAM.valid, result.maxDisk.valid, result.maxVCPU.valid, result.maxActiveVM.valid, result.maxTotVM.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.teamService.updateVM(new Team(team.id, team.name, result.maxRAM.value, result.maxDisk.value, result.maxVCPU.value, result.maxActiveVM.value, result.maxTotVM.value))
        .subscribe(authResult => {
            
          if(authResult === false){
            // not logged error message display
            this.updateVMInvalid = true
            this.openUpdateVMDialog(team)
          }else{

            // created
            this.updateVMInvalid = false
            this.maxRAM.reset()
            this.maxDisk.reset()
            this.maxTotVM.reset()
            this.maxVCPU.reset()
            this.maxActiveVM.reset()
            this.router.navigate(['/teacher/' + this.courseService.currentCourse.name + '/vms'])
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
        this.router.navigate(['/teacher/' + this.courseService.currentCourse.name + '/vms'])
      }
    }));
    
  }

  openUpdateCourseVMDialog(){

    let dialogRef = this.dialog.open(UpdateVMDialogComponent, {
      width: '400px',
      data: {maxRAM: this.maxRAM, maxDisk: this.maxDisk, maxVCPU: this.maxVCPU, maxActiveVM: this.maxActiveVM, maxTotVM: this.maxTotVM, updateVMInvalid: this.updateVMInvalid}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.maxRAM.valid, result.maxDisk.valid, result.maxVCPU.valid, result.maxActiveVM.valid, result.maxTotVM.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.teamService.updateCourseVM(result.maxRAM.value, result.maxDisk.value, result.maxVCPU.value, result.maxActiveVM.value, result.maxTotVM.value)
        .subscribe(authResult => {
            
          if(authResult === false){
            // not logged error message display
            this.updateVMInvalid = true
            this.openUpdateCourseVMDialog()
          }else{

            // created
            this.updateVMInvalid = false
            this.maxRAM.reset()
            this.maxDisk.reset()
            this.maxTotVM.reset()
            this.maxVCPU.reset()
            this.maxActiveVM.reset()
            this.router.navigate(['/teacher/' + this.courseService.currentCourse.name + '/vms'])
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
        this.router.navigate(['/teacher/' + this.courseService.currentCourse.name + '/vms'])
      }
    }));
    
  }

  ngOnDestroy(){
    this.subsciptions.unsubscribe()
  }

}
