import { Component, Input } from '@angular/core';
import { Course } from '../course.model';
import { CourseService } from '../services/course.service';

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

  constructor(private courseService: CourseService) { }

  setCurrentCourse(course: Course){
    this.courseService.currentCourse = course;
  }

}