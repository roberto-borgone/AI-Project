import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http'
import { Course } from '../course.model';
import { catchError } from 'rxjs/operators';
import { Observable, throwError, from } from 'rxjs'
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

  create(course: Course){
    this.http.post<Course>(this.API_PATH, course, this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  update(course: Course){
    this.http.put<Course>(this.API_PATH, course, this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
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

    let PATH = 'https://localhost:4200/api/API/students/' + this.auth.token.username + '/courses'
    //return of(this.students)
    return this.http.get<Course[]>(PATH)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  delete(id: number){
    this.http.delete(this.API_PATH + '/' + id)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

}
