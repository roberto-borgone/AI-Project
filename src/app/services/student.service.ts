import { Injectable } from '@angular/core'
import { Student} from 'src/app/student.model'
import { Observable, throwError, from } from 'rxjs'
import { HttpClient, HttpHeaders } from '@angular/common/http'
import { catchError, concatMap, toArray, map, flatMap, mergeAll } from 'rxjs/operators';
import { CourseService } from './course.service';
import { Team } from '../team.model';

export interface GroupEntity{
  id: number
  name: string
}

@Injectable({
  providedIn: 'root'
})
export class StudentService {

  private API_PATH = 'https://localhost:4200/api/API/students'

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }

  constructor(private http: HttpClient, private courseService: CourseService) {}

  create(student: Student){
    this.http.post<Student>(this.API_PATH, student, this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  update(student: Student){
    this.http.put<Student>(this.API_PATH, student, this.httpOptions)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  find(id: number): Observable<Student>{
    return this.http.get<Student>(this.API_PATH + '/' + id)
    .pipe(
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }

  query(): Observable<Student[]>{
    //return of(this.students)
    return this.http.get<Student[]>(this.API_PATH)
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

  enroll(student: Student): Observable<boolean>{
    return this.http.post<Object>(this.courseService.API_PATH + '/' + this.courseService.currentCourse.name + '/enrollOne', {id: student.id}, this.httpOptions)
    .pipe(
      map(result => {return true})
    )
  }

  deleteFromCourse(students: Student[]): Observable<Student[]>{
    return from(students).pipe(
      concatMap(student => {
        return this.http.post<Student>(this.courseService.API_PATH + '/' + this.courseService.currentCourse.name + '/deleteStudentFromCourse/' + student.id, student, this.httpOptions)
      }),
      toArray()
    )
  }

  getEnrolled(courseId: string): Observable<Student[]>{

    let endpoint: string = 'https://localhost:4200/api/API/courses' + '/' + courseId + '/' + 'enrolled'

    /*return this.http.get<StudentEntity[]>(endpoint)
    .pipe(
      map(students => {
        let studentsDTO: Student[] = []
        students.forEach(student => studentsDTO.push(new Student(student.id, 
                                                                 student.name, 
                                                                 student.surname,
                                                                 student.group?student.group.name:'<none>')))
        return studentsDTO
      }),
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    ) */
    
    return this.http.get<Student[]>(endpoint)
    .pipe(
      mergeAll(),
      concatMap(student => {
        return this.http.get<GroupEntity>(this.API_PATH + '/' + courseId + '/' + student.id + '/getTeam').pipe(
          map(team => {
            if(team)
              student.group = team.name
            else
              student.group = ''
            return student
          })
      )}),
      toArray(),
      catchError( err => {
        console.error(err)
        return throwError(err.message)
      })
    )
  }
}