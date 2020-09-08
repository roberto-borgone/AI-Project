import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http'
import { Course } from '../course.model';
import { catchError, map } from 'rxjs/operators';
import { Observable, throwError, of } from 'rxjs'
import { AuthService } from '../auth/auth.service';

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

    if(this.auth.token.username[0]=='s')
      PATH = 'https://localhost:4200/api/API/students/' + this.auth.token.username + '/courses'
    else
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

}
