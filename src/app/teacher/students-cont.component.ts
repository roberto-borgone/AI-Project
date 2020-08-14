import { Component, OnDestroy } from '@angular/core';
import { Student} from 'src/app/student.model'
import { StudentService } from '../services/student.service';
import { Subscription } from 'rxjs';
import { CourseService } from '../services/course.service';
import { Router, NavigationEnd } from '@angular/router';

@Component({
  selector: 'app-students-cont',
  templateUrl: './students-cont.component.html',
  styleUrls: ['./students-cont.component.css']
})
export class StudentsContComponent implements OnDestroy{

  enrolledStudents: Student[]
  students: Student[]
  subscriptions: Subscription = new Subscription()

  constructor(private studentService: StudentService, private courseService: CourseService, private router: Router) {
    this.getStudents()
    this.getEnrolledStudents()
    this.subscriptions.add(this.router.events.subscribe(val => {
      if(val instanceof NavigationEnd){
        this.getStudents()
        this.getEnrolledStudents()
      }
    }))
  }

  deleteStudent(students: Student[]){
    this.subscriptions.add(this.studentService.updateEnrolled(0, students).subscribe(x => this.getEnrolledStudents()))
  }

  addStudent(students: Student[]){
    this.subscriptions.add(this.studentService.updateEnrolled(this.courseService.currentCourse.id, students).subscribe(x => this.getEnrolledStudents()))
  }

  getStudents(){
    this.subscriptions.add(this.studentService.query().subscribe(students => this.students = students))
  }

  getEnrolledStudents(){
    this.subscriptions.add(this.studentService.getEnrolled(this.courseService.currentCourse.id).subscribe(students => this.enrolledStudents = students))
  }

  uploadFile(file: any) {
    console.log("Sono in uploadFile in students-cont");
    console.log(file);
    this.studentService.uploadFile(file);
  }

  ngOnDestroy(){
    this.subscriptions.unsubscribe()
  }
}
