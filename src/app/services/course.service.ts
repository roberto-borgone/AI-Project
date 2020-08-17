import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http'
import { Course } from '../course.model';
import { catchError } from 'rxjs/operators';
import { Observable, throwError, from } from 'rxjs'

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
     
  constructor(private http: HttpClient) {}

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
    //return of(this.students)
    return this.http.get<Course[]>(this.API_PATH)
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
