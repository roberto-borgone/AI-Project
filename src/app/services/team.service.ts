import { Injectable } from '@angular/core';
import { CourseService } from './course.service';
import { Observable, throwError, of } from 'rxjs';
import { catchError, map, concatMap } from 'rxjs/operators';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { AuthService } from '../auth/auth.service';
import { Team } from '../models/team.model';
import { Student } from '../models/student.model';
import { Proposal } from '../models/proposal.model';

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

  constructor(private http: HttpClient,private courseService: CourseService, private auth: AuthService) { }

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

  updateVM(team: Team): Observable<any>{
    return this.http.post<Object>(this.API_PATH + '/' + team.id + '/newTeamLimits', {maxRAM: team.maxRAM, maxDisk: team.maxDisk, maxVCPU: team.maxVCPU, maxTotVM: team.maxTotVM, maxActiveVM: team.maxActiveVM}, this.httpOptions)
    .pipe(
      map(result => {
        return of(result)
      }),
      catchError(err => {
        return of(err)
      })
    )
  }

  updateCourseVM(maxRAM : number, maxDisk : number, maxVCPU : number, maxActiveVM : number, maxTotVM : number): Observable<any>{

    let PATH = 'https://localhost:4200/api/API/courses'

    return this.http.post<Object>(PATH + '/' + this.courseService.currentCourse.name + '/updateLimits', {maxRAM: maxRAM, maxDisk: maxDisk, maxVCPU: maxVCPU, maxTotVM: maxTotVM, maxActiveVM: maxActiveVM}, this.httpOptions)
    .pipe(
      map(result => {
        return of(result)
      }),
      catchError(err => {
        return of(err)
      })
    )
  }

  getMembers(): Observable<Student[]>{
    return this.courseService.getGroup().pipe(
      concatMap(result => {
        if(result){
          let resultQuery: Student[]
          return this.http.get<Student[]>(this.API_PATH + '/' + this.auth.token.group.id, this.httpOptions)
        }else{
          let resultQuery: Student[] = []
          return of(resultQuery)
        }
        })
    )
  }

  getTeamPropStudent(): Observable<Student[]>{
    return this.courseService.getGroup().pipe(
      concatMap(result => {
        if(result){
          let resultQuery: Student[]
          return this.http.get<Student[]>(this.API_PATH + '/' + this.auth.token.group.id + "/" + "teamProposal", this.httpOptions)
        }else{
          let resultQuery: Student[] = []
          return of(resultQuery)
        }
        })
    )
  }

  getStudents(): Observable<Student[]>{
    
    let PATH = 'https://localhost:4200/api/API/courses'
    
    return this.courseService.getGroup().pipe(
      concatMap(result => {
        if(result){
          let resultQuery: Student[] = []
          return of(resultQuery)
        }else{
          return this.http.get<Student[]>(PATH + '/' + this.courseService.currentCourse.name + '/availablestudents', this.httpOptions)
        }
      })
    )
  }
  
  getProposal(): Observable<Proposal[]>{
    let PATH = 'https://localhost:4200/api/API/students'

    return this.courseService.getGroup().pipe(
      concatMap(result => {
        if(!result || this.auth.token.groupStatus == 0){
          let resultQuery: Proposal[]
          return this.http.get<Proposal[]>(PATH + "/" + this.courseService.currentCourse.name + "/proposal", this.httpOptions)
        }else{
          let resultQuery: Proposal[] = []
          return of(resultQuery)
        }
        })
    )
  }

  newTeam(name: string, students: Student[]){
    
    let PATH = 'https://localhost:4200/api/API/courses'

    let studentsID: string[] = []

    for(let student of students){
      studentsID.push(student.id)
    }

    studentsID.push(this.auth.token.username)

    return this.http.post<Object>(PATH + '/' + this.courseService.currentCourse.name + '/proposeTeam', {nameTeam: name, memberIds: studentsID, timeout: 10}, this.httpOptions)
    .pipe(
      map(result => {
        console.log(result)
        return of(result)
      }),
      catchError(err => {
        console.log(err)
        return of(err)
      })
    )
  }
  
  accept(id: number): Observable<any>{
    let PATH = 'https://localhost:4200/api/API/teams/accept'

    return this.http.get<Object>(PATH + '/' + id)
    .pipe(
      map(result => {
        console.log(result)
        return of(result)
      }),
      catchError(err => {
        console.log(err)
        return of(err)
      })
    )
  }

  reject(id: number): Observable<any>{
    let PATH = 'https://localhost:4200/api/API/teams/reject'

    return this.http.get<Object>(PATH + '/' + id)
    .pipe(
      map(result => {
        return of(result)
      }),
      catchError(err => {
        return of(err)
      })
    )
  }
}
