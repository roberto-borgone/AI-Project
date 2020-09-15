import { Component, ViewChild, ElementRef, Input, Output, OnInit } from '@angular/core';
import {MatTableDataSource} from '@angular/material/table';
import { MatSort } from '@angular/material/sort';
import { MatPaginator } from '@angular/material/paginator';
import { Student} from 'src/app/student.model'
import { SelectionModel } from '@angular/cdk/collections';
import { MatAutocompleteSelectedEvent } from '@angular/material/autocomplete';
import { EventEmitter } from '@angular/core';

@Component({
  selector: 'app-students',
  templateUrl: './students.component.html',
  styleUrls: ['./students.component.css']
})
export class StudentsComponent {

  @ViewChild('input')
  input: ElementRef

  @ViewChild(MatSort, {static: true})
  sort: MatSort

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  @Output()
  onDelete: EventEmitter<Student[]>

  @Output()
  onAdd: EventEmitter<Student>

  @Output()
  onFileUpload: EventEmitter<any>

  // column to display in table
  colsToDisplay: string[] = ['select', 'id', 'name', 'surname', 'group']

  // selected student
  selectedStudent: Student
  // chackbox selection
  selection: SelectionModel<Student>
  
  // table data supplied by enrolledStudentSource
  enrolledStudents: MatTableDataSource<Student>

  //array of enrolled students
  enrolledStudentInCourse: Student[]

  @Input()
  set _enrolledStudents(students: Student[]){
    this.enrolledStudents = new MatTableDataSource(students)
    this.enrolledStudents.sort = this.sort
    this.enrolledStudents.paginator = this.paginator
    this.enrolledStudentInCourse = students
  }

  students: Student[]
  // students filtrated by search filter
  filteredStudents: Student[]

  @Input()
  set _students(students: Student[]){
    this.students = students
    this.filteredStudents = this.students
  }

  constructor(){
    this.selection = new SelectionModel<Student>(true, [])
    this.onDelete = new EventEmitter()
    this.onAdd = new EventEmitter()
    this.onFileUpload = new EventEmitter()
  }

  // row selection toggle
  toggleSelectionRow(event: Event, student: Student) {
    this.selection.toggle(student)
  }

  // condition for a checked row
  isRowSelected(student: Student): boolean {
    return this.selection.isSelected(student)
  }

  // check if all row selected
  isAllSelected() {
    // number of selected equal to total enrolled students
    const numSelected = this.selection.selected.length;
    const numRows = this.enrolledStudents.data.length;
    return numSelected === numRows;
  }

  // master toggle handling
  masterToggle() {
    // clear all if already all selected otherwise select all
    this.isAllSelected() ?
        this.selection.clear() :
        this.enrolledStudents.data.forEach(row => this.selection.select(row));
  }

  // condition for master checked
  isMasterChecked(): boolean {
    // master is checked if all the students are checked
    return this.selection.hasValue() && this.isAllSelected()
  }

  // condition for master indeterminate
  isMasterIndeterminate(): boolean {
    // if some students are selected but not all master is indeterminate
    return this.selection.hasValue() && !this.isAllSelected()
  }

  // condition for master disabled
  isMasterDisabled(): boolean {
    // if there are no students the master toggle is disabled
    return this.enrolledStudents.data.length === 0
  }

  // delete selection
  deleteSelected() {
    this.onDelete.emit(this.selection.selected)
    this.selection = new SelectionModel<Student>(true, [])
  }

  // string representation of a student
  studentToString(student: Student): string {
    return student.name.toUpperCase() + ' ' + student.surname.toUpperCase() + ' (' + student.id + ')'
  }

  // filter options in autocomplete
  filterStudents() {

    let filter: string = this.input.nativeElement.value

    // the input matches if is present in name or first name 
    // or if it matches the id beginning
    filter = filter.toUpperCase()
    this.filteredStudents = this.students.filter(s => 
      (s.name.toUpperCase().includes(filter) || 
      s.name.toUpperCase().includes(filter) || 
      s.id.startsWith(filter)))
  }

  // retrieve user option selection
  getOption(event: MatAutocompleteSelectedEvent){
    this.selectedStudent = event.option.value
  }

  // add user selection to table
  addStudent() {

    // avoid multiple insertion
    if(this.selectedStudent != null && this.enrolledStudents.data.findIndex(d => d.id == this.selectedStudent.id) === -1) {
      this.onAdd.emit(this.selectedStudent)
      // clear the input field
      this.input.nativeElement.value = ''
      // update the autocomplete options and invalidate the selection
      this.filterStudents()
      this.selectedStudent = null
    }
  }

  onClick() { 
    const fileUpload = document.getElementById('fileUpload') as HTMLInputElement;
    fileUpload.click();
}

  handleFileSelect(event: any) {
    console.log("Sono in handleFileSelect");
    var files = event.target.files; // FileList object
    var file = files[0];
    this.onFileUpload.emit(file);
    /*
    var reader = new FileReader();

    reader.readAsText(file);
    reader.onload = (event: any) => {
      var csv = event.target.result; // Content of CSV file
      console.log(csv);        
    }
    */ 
  }
}
