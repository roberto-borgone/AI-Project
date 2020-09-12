import { Component, OnDestroy } from '@angular/core';
import { Course } from '../course.model';
import { Subscription } from 'rxjs';
import { CourseService } from '../services/course.service';

@Component({
  selector: 'app-student-course',
  templateUrl: './student-course.component.html',
  styleUrls: ['./student-course.component.css']
})
export class StudentCourseComponent implements OnDestroy {

  subscriptions: Subscription = new Subscription()
  courses: Course[]

  constructor(private courseService: CourseService) { 
    this.getCourses()
  }

  getCourses(){
    this.subscriptions.add(this.courseService.query().subscribe(courses => this.courses = courses))
  }

  setCurrentCourse(course: Course){
    this.courseService.currentCourse = course;
  }

  ngOnDestroy(): void {
    this.subscriptions.unsubscribe()
  }

}
