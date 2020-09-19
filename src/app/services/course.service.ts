import { Injectable, OnDestroy } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http'
import { Course } from '../models/course.model';
import { catchError, filter, map, switchMap } from 'rxjs/operators';
import { Observable, throwError, of, Subscription } from 'rxjs'
import { ModelVM } from '../models/modelVM.model';
import { ActivatedRoute, Event, NavigationEnd, Router } from '@angular/router';

@Injectable({
  providedIn: 'root'
})
export class CourseService implements OnDestroy{

  API_PATH = 'https://localhost:4200/api/API/courses'

  courses: Course[]
  currentCourse: Course
  navigationEnd: Observable<Event>
  subscriptions: Subscription = new Subscription()

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }
     
  constructor(private http: HttpClient, private route: ActivatedRoute, private router: Router) {

    this.navigationEnd = this.router.events.pipe(
      filter(event => event instanceof NavigationEnd)
    );

    this.subscriptions.add(this.navigationEnd
      .pipe(
        map(() => {return this.route.root.firstChild}),
        switchMap((firstChild: ActivatedRoute) => {
          if(firstChild && firstChild.firstChild){
            return firstChild.firstChild.paramMap
            .pipe(map(paramMap => {return paramMap.get('id')}))
          }
          else{
            return of(undefined)
          }
        }
        )
      ).subscribe((result: string) => {
        if(this.courses){
          this.currentCourse = this.courses.find(c => c.name == decodeURI(result))
        }
      }))
  }

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

  query(role: string, username: string): Observable<Course[]>{

    let PATH : string;

    if(role == 'student')
      PATH = 'https://localhost:4200/api/API/students/' + username + '/courses'
    else if(role == 'teacher')
      PATH = 'https://localhost:4200/api/API/docents/' + username + '/courses'

    //return of(this.students)
    return this.http.get<Course[]>(PATH)
    .pipe(
      map(result => {
        this.courses = result
        return result
      }),
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

  ngOnDestroy(){
    this.subscriptions.unsubscribe()
  }
  
}
