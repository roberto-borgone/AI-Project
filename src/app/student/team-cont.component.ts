import { Component, OnDestroy, AfterViewInit, OnInit } from '@angular/core';
import { AuthService } from '../auth/auth.service';
import { Subscription } from 'rxjs';
import { TeamService } from '../services/team.service';
import { NavigationEnd, Router } from '@angular/router';
import { MatDialog } from '@angular/material/dialog';
import { FormControl, Validators } from '@angular/forms';
import { NewTeamDialogComponent } from './new-team-dialog.component';
import { Student } from '../models/student.model';
import { Proposal } from '../models/proposal.model';


@Component({
  selector: 'app-team-cont',
  templateUrl: './team-cont.component.html',
  styleUrls: ['./team-cont.component.css']
})
export class TeamContComponent implements OnDestroy{

  team: Student[]
  studentInProposal: Student[]
  enrolledStudents: Student[]

  proposal: Proposal[]
  subscriptions: Subscription = new Subscription()

  newTeamName: FormControl = new FormControl('', [Validators.required])
  timeout: FormControl = new FormControl('', [Validators.min(1)])
  newTeamInvalid: boolean = false
  message: string

  constructor(private auth: AuthService, private teamService: TeamService, private router: Router, private dialog: MatDialog) {
      this.subscriptions.add(this.router.events.subscribe(val => {
        if(val instanceof NavigationEnd){
          this.getStudents()
          this.getTeamMembers()
          this.getProposals()
          this.getTeamPropStudent()
        }
      }))
  }

  getTeamMembers(){
    this.subscriptions.add(this.teamService.getMembers().subscribe(result => {
      this.team = result
    }))
  }

  getTeamPropStudent(){
    this.subscriptions.add(this.teamService.getTeamPropStudent().subscribe(result => {
      this.studentInProposal = result
    }))
  }

  getStudents(){
    this.subscriptions.add(this.teamService.getStudents().subscribe(result => {
      var s: Student[] = new Array()
      result.forEach(element => {
        if(element.id != this.auth.token.username){
          s.push(element)
        }
      });
      this.enrolledStudents = s
    }))
  }

  openDialogTeamUp(students: Student[]){
    let dialogRef = this.dialog.open(NewTeamDialogComponent, {
      width: '400px',
      data: {students: students, newTeamName: this.newTeamName, timeout: this.timeout, newTeamInvalid: this.newTeamInvalid, message: this.message}
    });

    this.subscriptions.add(dialogRef.afterClosed().subscribe(result => {
      if(result && result.newTeamName.valid && result.timeout.valid){

        // nested observables.. i could have found a more elegant solution to this
        this.subscriptions.add(this.teamService.newTeam(result.newTeamName.value, result.timeout.value, students)
        .subscribe(authResult => {
            
          if(authResult.ok == false){
            // not logged error message display
            this.newTeamInvalid = true
            this.message = authResult.error.message
            this.openDialogTeamUp(students)
          }else{

            // created
            this.newTeamInvalid = false
            this.newTeamName.reset()
            this.timeout.reset()
            this.message = undefined
            this.getTeamMembers()
            this.getStudents()
            this.getProposals()
            this.getTeamPropStudent()
          }
        }))
      }else if(!result){
        // dialog closed i remove data
        this.newTeamInvalid = false
        this.newTeamName.reset()
        this.timeout.reset()
        this.message = undefined
      }
    }));
  }

  getProposals(){
    this.subscriptions.add(this.teamService.getProposal().subscribe(result => {
      this.proposal = result
    }))
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe()
  }

  onAccept(id: number){
    console.log(id)
    this.subscriptions.add(this.teamService.accept(id).subscribe(result => {
      console.log(result)
      this.getTeamMembers()
      this.getProposals()
      this.getTeamPropStudent()
    }))
    
  }

  onReject(id: number){
    console.log(id)
    this.subscriptions.add(this.teamService.reject(id).subscribe(result => {
      
      console.log(result)
      this.getStudents()
      this.getTeamMembers()
      this.getProposals()
      this.getTeamPropStudent()

    }))
    

  }

}
