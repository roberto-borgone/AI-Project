import { Proposal } from './../proposal.model';
import { Component, OnDestroy, AfterViewInit, OnInit } from '@angular/core';
import { Student } from '../student.model';
import { AuthService } from '../auth/auth.service';
import { Subscription } from 'rxjs';
import { TeamService } from '../services/team.service';
import { NavigationEnd, Router } from '@angular/router';

@Component({
  selector: 'app-team-cont',
  templateUrl: './team-cont.component.html',
  styleUrls: ['./team-cont.component.css']
})
export class TeamContComponent implements OnDestroy{

  team: Student[]
  proposal: Proposal[]
  subscriptions: Subscription = new Subscription()

  constructor(private auth: AuthService, private teamService: TeamService, private router: Router) {
      this.subscriptions.add(this.router.events.subscribe(val => {
        if(val instanceof NavigationEnd){
          this.getTeamMembers()
          this.getProposals()
        }
      }))
  }

  getTeamMembers(){
    this.subscriptions.add(this.teamService.getMembers().subscribe(result => {
      this.team = result
    }))
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
    this.subscriptions.add(this.teamService.accept(id).subscribe(result => console.log(result)))
    this.getTeamMembers()
    this.getProposals()
  }

  onReject(id: number){
    console.log(id)
    this.subscriptions.add(this.teamService.reject(id).subscribe(result => console.log(result)))
    this.getTeamMembers()
    this.getProposals()

  }

}
