import { Component, Input, Output } from '@angular/core';
import { Course } from '../course.model';
import { CourseService } from '../services/course.service';
import { EventEmitter } from '@angular/core';

@Component({
  selector: 'app-courses',
  templateUrl: './courses.component.html',
  styleUrls: ['./courses.component.css']
})
export class CoursesComponent {

  courses: Course[]

  @Input()
  set _courses(courses: Course[]){
    this.courses = courses
  }

  @Output()
  toggleCourseEvent: EventEmitter<Course>

  constructor(private courseService: CourseService) {
    this.toggleCourseEvent = new EventEmitter()
   }

  setCurrentCourse(course: Course){
    this.courseService.currentCourse = course;
  }

  toggleCourse(course: Course){
    this.toggleCourseEvent.emit(course)
  }

}
