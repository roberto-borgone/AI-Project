import { Component, ElementRef, Inject, Input, ViewChild } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Router } from '@angular/router';
import { MAT_DIALOG_DATA, MatDialogRef } from '@angular/material/dialog';
import { Teacher } from '../models/teacher.model';
import { MatAutocompleteSelectedEvent } from '@angular/material/autocomplete';
import { Subscription } from 'rxjs';
import { CourseService } from '../services/course.service';
import { AuthService } from '../auth/auth.service';

export interface DialogData {
  courseName: FormControl;
  courseAcronimo: FormControl;
  courseMin: FormControl;
  courseMax: FormControl;
  courseEnabled: FormControl;
  courseNameVM: FormControl;
  courseVersionVM: FormControl;
  addCourseInvalid: boolean;
  owners: Teacher[]
}

@Component({
  selector: 'app-add-course-dialog',
  templateUrl: './add-course-dialog.component.html',
  styleUrls: ['./add-course-dialog.component.css']
})
export class AddCourseDialogComponent {

  @ViewChild('input')
  input: ElementRef

  selectedTeacher: Teacher

  teachers: Teacher[]
  // students filtrated by search filter
  filteredTeachers: Teacher[]

  subscriptions: Subscription = new Subscription()

  constructor(private router: Router,
    public dialogRef: MatDialogRef<AddCourseDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData, private courseService: CourseService, public auth: AuthService) {

      this.getTeachers()
  }

  sistemiOp: string[] = ['ios', 'ubuntu', 'windows'];

  onNoClick(): void {
    this.dialogRef.close();
  }

  getErrorMessageName(courseName: FormControl) {
    if (courseName.hasError('required')) {
      return 'Inserisci un nome';
    }
  }

  getErrorMessageAcronimo(courseAcronimo: FormControl) {
    if (courseAcronimo.hasError('required')) {
      return 'Inserisci un acronimo';
    }
  }

  getErrorMessageMin(courseMin: FormControl) {
    if (courseMin.hasError('required')) {
      return 'Inserisci un numero valido di studenti';
    }
  }

  getErrorMessageMax(courseMax: FormControl) {
    if (courseMax.hasError('required')) {
      return 'Inserisci un numero valido di studenti';
    }
  }

  getErrorMessageNameVM(courseNameVM: FormControl) {
    if (courseNameVM.hasError('required')) {
      return 'Inserisci un sistema operativo';
    }
  }

  getErrorMessageVersionVM(courseVersionVM: FormControl) {
    if (courseVersionVM.hasError('required')) {
      return 'Inserisci una versione';
    }
  }

  teacherToString(teacher: Teacher): string {
    return teacher.name.toUpperCase() + ' ' + teacher.surname.toUpperCase() + ' (' + teacher.id + ')'
  }

  filterTeachers() {

    let filter: string = this.input.nativeElement.value

    // the input matches if is present in name or first name 
    // or if it matches the id beginning
    filter = filter.toUpperCase()
    this.filteredTeachers = this.filteredTeachers.filter(s => 
      (s.name.toUpperCase().includes(filter) || 
      s.name.toUpperCase().includes(filter) || 
      s.id.startsWith(filter)))
  }

  // retrieve user option selection
  getOption(event: MatAutocompleteSelectedEvent){
    this.selectedTeacher = event.option.value
    this.addTeacher()
  }

  // add user selection to table
  addTeacher() {

    // avoid multiple insertion
    if(this.selectedTeacher != null && this.data.owners.findIndex(d => d.id == this.selectedTeacher.id) === -1) {
      this.data.owners.push(this.selectedTeacher)
      this.filteredTeachers = this.filteredTeachers.filter(d => d.id != this.selectedTeacher.id)
      // clear the input field
      this.input.nativeElement.value = ''
      this.input.nativeElement.blur()
      // update the autocomplete options and invalidate the selection
      this.filterTeachers()
      this.selectedTeacher = null
    }
  }

  getTeachers(){
    this.subscriptions.add(this.courseService.getTeachers().subscribe(result => {
      
      //console.log(result)

      this.teachers = result
      this.teachers = this.teachers.filter(d => d.id != this.auth.token.username)
      this.filteredTeachers = this.teachers

      console.log(this.filteredTeachers)
    
    }))
  }

  removeTeacher(teacher: Teacher){
    this.data.owners = this.data.owners.filter(d => d.id != teacher.id)
    this.filteredTeachers.push(teacher)
    this.filterTeachers()
  }
}
