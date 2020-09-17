import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, of, throwError } from 'rxjs';
import { catchError, map } from 'rxjs/operators';
import { Assignment } from '../models/assignment.model';
import { AuthService } from '../auth/auth.service';
import { LastWork } from '../models/last-work.model';
import { CourseService } from './course.service';
import { Work } from '../models/work.model';

@Injectable({
  providedIn: 'root'
})
export class AssignmentService {

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }

  constructor(private http: HttpClient, private auth: AuthService, private courseService: CourseService) { }

  getStudentsWorks(assignmentId : number): Observable<any> {

    let PATH = 'https://localhost:4200/api/API/consegne/';
    return this.http.get<LastWork[]>(PATH + assignmentId + '/' + this.auth.token.username + '/getAllStatus')
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  getStudentHistory(assignmentId : number, studentId : string): Observable<any> {
    
    let PATH = 'https://localhost:4200/api/API/consegne/';
    return this.http.get<Work[]>(PATH + assignmentId + '/' + studentId + '/' + this.auth.token.username + '/getAllStudentElaborati')
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  createAssignment(dueDate: Date, file: File): Observable<any> {

    console.log("Sono in createAssignmentService")
    console.log(dueDate)
    console.log(file)

    const formData = new FormData();
    formData.append('imagefile', file);

    let PATH = 'https://localhost:4200/api/API/courses/';
    
    return this.http.post<any>(PATH + this.courseService.currentCourse.name + '/' + this.auth.token.username + '/' + dueDate + '/newConsegna', formData)
    .pipe(
      map(result => {
        return true}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

  getAssignmentContent(assignment: Assignment): Observable<any> {

    let PATH = 'https://localhost:4200/api/API/courses/';

    const requestOptions: Object = {
      responseType: 'blob'
    }

    return this.http.get<any>(PATH + this.courseService.currentCourse.name + '/' + assignment.id + '/getConsegna', requestOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  uploadCorrection(assignmentId: number, studentId: string, file: any) {

    const formData = new FormData();
    formData.append('imagefile', file);

    let PATH = 'https://localhost:4200/api/API/consegne/';
    
    return this.http.post<any>(PATH + + assignmentId + '/' + studentId + '/' + this.auth.token.username + '/consegnaRevisione', formData)
    .pipe(
      map(result => {
        return true}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

  setVote(assignmentId: number, studentId: string, vote: number, laude: boolean) {

    let PATH = 'https://localhost:4200/api/API/consegne/';
    
    return this.http.post<any>(PATH + assignmentId + '/' + studentId + '/' + this.auth.token.username + '/setVoto', {'voto': vote, 'lode': laude})
    .pipe(
      map(result => {
        return true}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

  getWorkContent(workId: number) {

    let PATH = 'https://localhost:4200/api/API/consegne/';

    const requestOptions: Object = {
      responseType: 'blob'
    }

    return this.http.get<any>(PATH + workId + '/getElaborato', requestOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  disableAssignment(assignmentId: number, studentId: string) {

    let PATH = 'https://localhost:4200/api/API/consegne/';
    
    return this.http.get<any>(PATH + assignmentId + '/' + this.auth.token.username + '/' + studentId + '/updateStatusConsegna')
    .pipe(
      map(result => {
        return true}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

  getStudentWorks(assignment: Assignment) {

    let PATH = 'https://localhost:4200/api/API/consegne/';
    return this.http.get<Work[]>(PATH + assignment.id + '/' + this.auth.token.username + '/' + assignment.docent_id + '/getAllStudentElaborati')
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }
}
