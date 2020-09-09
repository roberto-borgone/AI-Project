import { Injectable } from '@angular/core';
import { CourseService } from './course.service';
import { Team } from '../team.model';
import { Observable, throwError, of } from 'rxjs';
import { catchError, map } from 'rxjs/operators';
import { HttpClient, HttpHeaders } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class TeamService {

  API_PATH = 'https://localhost:4200/api/API/teams'

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }

  constructor(private http: HttpClient,private courseService: CourseService) { }

  query(): Observable<Team[]>{

    let PATH = 'https://localhost:4200/api/API/courses'

    return this.http.get<Team[]>(PATH + '/' + this.courseService.currentCourse.name + '/teams', this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  updateVM(team: Team): Observable<boolean>{
    return this.http.post<Object>(this.API_PATH + '/' + team.id + '/newTeamLimits', {maxRAM: team.maxRAM, maxDisk: team.maxDisk, maxVCPU: team.maxVCPU, maxTotVM: team.maxTotVM, maxActiveVM: team.maxActiveVM}, this.httpOptions)
    .pipe(
      map(result => {
        return true
      }),
      catchError(err => {
        console.error(err)
        return of(false)
      })
    )
  }
}
