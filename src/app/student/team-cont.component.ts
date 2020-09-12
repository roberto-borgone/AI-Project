import { Component, OnDestroy, AfterViewInit, OnInit } from '@angular/core';
import { Student } from '../student.model';
import { AuthService } from '../auth/auth.service';
import { Subscription } from 'rxjs';
import { TeamService } from '../services/team.service';

@Component({
  selector: 'app-team-cont',
  templateUrl: './team-cont.component.html',
  styleUrls: ['./team-cont.component.css']
})
export class TeamContComponent implements OnDestroy{

  team: Student[]
  subscriptions: Subscription = new Subscription()

  constructor(private auth: AuthService, private teamService: TeamService) {
      this.getTeamMembers()
  }

  getTeamMembers(){
    this.subscriptions.add(this.teamService.getMembers().subscribe(result => {
      this.team = result
    }))
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe()
  }

}
