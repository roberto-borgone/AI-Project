import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http'
import { Course } from '../course.model';
import { catchError, map } from 'rxjs/operators';
import { Observable, throwError, of } from 'rxjs'
import { AuthService } from '../auth/auth.service';
import { ModelVM } from '../teacher/modelVM.model';
import { Assignment } from '../assignment.model';
import { Team } from '../team.model';

@Injectable({
  providedIn: 'root'
})
export class CourseService {

  API_PATH = 'https://localhost:4200/api/API/courses'

  currentCourse: Course

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }
     
  constructor(private http: HttpClient, private auth: AuthService) {}

  create(course: Course): Observable<Boolean>{
    return this.http.post<Course>(this.API_PATH, course, this.httpOptions)
    .pipe(
      map(result => {
        return true}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

  update(course: Course): Observable<boolean>{
    return this.http.put<Course>(this.API_PATH + '/' + course.name, course, this.httpOptions)
    .pipe(
      map(result => {
        return true
      }),
      catchError( err => {
        console.error(err)
        return of(false)
      })
    )
  }

  find(id: number): Observable<Course>{
    return this.http.get<Course>(this.API_PATH + '/' + id)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  query(): Observable<Course[]>{

    let PATH : string;

    if(this.auth.token.role == 'student')
      PATH = 'https://localhost:4200/api/API/students/' + this.auth.token.username + '/courses'
    else if(this.auth.token.role == 'teacher')
      PATH = 'https://localhost:4200/api/API/docents/' + this.auth.token.username + '/courses'

    //return of(this.students)
    return this.http.get<Course[]>(PATH)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  updateStatus(course: Course): Observable<boolean>{
    return this.http.post(this.API_PATH + '/' + course.name + '/updateStatus', {enabled: !course.enabled}, this.httpOptions)
    .pipe(
      map(result => {
        return true}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

  deleteCourse(course: Course): Observable<boolean>{
    return this.http.delete<boolean>(this.API_PATH + '/' + course.name, this.httpOptions)
    .pipe(
      map(result => {
        return result}),
      catchError( err => {
        console.log(err)
        return of(false)
      })
    )
  }

  uploadFile(file: any) : Observable<any> {
    console.log("Sono in uploadFile in student service");

    const formData = new FormData();
    formData.append('file', new Blob([file], { type: 'text/csv' }), file.name);

    const API_PATH = 'https://localhost:4200/api/API/courses/'+this.currentCourse.name+'/enrollMany';

    return this.http.post<any>(API_PATH, formData)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  createModelVM(course: string, courseNameVM: string, courseVersionVM: string): Observable<boolean>{
    return this.http.post<Object>(this.API_PATH + '/' + course + '/modelVM', {name: courseNameVM, version: courseVersionVM}, this.httpOptions)
    .pipe(
      map(result => {
        return true
      }),
      catchError( err => {
        console.error(err)
        return of(false)
      })
    )
  }

  getModelVM() : Observable<any> {
    return this.http.get<ModelVM>(this.API_PATH + '/' + this.currentCourse.name + '/getModelVM')
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      }))
    }

  getCourse() : Observable<any> {
    return this.http.get<Course>(this.API_PATH + '/' + this.currentCourse.name)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  getAssignmentsService() : Observable<any> {
    return this.http.get<Assignment[]>(this.API_PATH + '/' + this.currentCourse.name + '/getConsegne')
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  getGroup(): Observable<boolean>{

    let PATH = 'https://localhost:4200/api/API/students/' + this.currentCourse.name + '/' + this.auth.token.username + '/getTeam'
    return this.http.get<Team>(PATH, this.httpOptions)
    .pipe(
      map(result => {
        
        if(result){
          this.auth.token.group = result
          return true
        }else{
          this.auth.token.group = undefined
          return false
        }
      }),
      catchError( err => {
        console.error(err)
        return of(false)
      })
    )
  }
}
