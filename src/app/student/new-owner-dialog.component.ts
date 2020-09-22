import { Component, ElementRef, Inject, OnInit, ViewChild } from '@angular/core';
import { MatAutocompleteSelectedEvent } from '@angular/material/autocomplete';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { Router } from '@angular/router';
import { Subscription } from 'rxjs';
import { AuthService } from '../auth/auth.service';
import { Student } from '../models/student.model';
import { VM } from '../models/vm.model';
import { TeamService } from '../services/team.service';

export interface DialogData {
  owners: Student[]
  vm: VM
}

@Component({
  selector: 'app-new-owner-dialog',
  templateUrl: './new-owner-dialog.component.html',
  styleUrls: ['./new-owner-dialog.component.css']
})
export class NewOwnerDialogComponent {

  @ViewChild('input')
  input: ElementRef

  selectedStudent: Student

  students: Student[]

  // students filtrated by search filter
  filteredStudents: Student[]

  subscriptions: Subscription = new Subscription()

  constructor(private router: Router,
    public dialogRef: MatDialogRef<NewOwnerDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData, public auth: AuthService, private teamService: TeamService) {
      this.getTeamMembers()
  }

  getTeamMembers(){
    this.subscriptions.add(this.teamService.getMembers().subscribe(result => {
      this.students = result
      this.students = this.students.filter(s => this.data.vm.owners.find(o => o.id == s.id) == undefined)
      this.filteredStudents = this.students
    }))
  }

  onNoClick(): void {
    this.dialogRef.close();
  }

  studentToString(student: Student): string {
    return student.name.toUpperCase() + ' ' + student.surname.toUpperCase() + ' (' + student.id + ')'
  }

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
    this.addStudent()
  }

  // add user selection to table
  addStudent() {

    // avoid multiple insertion
    if(this.selectedStudent != null && this.data.owners.findIndex(d => d.id == this.selectedStudent.id) === -1) {
      this.data.owners.push(this.selectedStudent)
      this.students = this.students.filter(d => d.id != this.selectedStudent.id)
      // clear the input field
      this.input.nativeElement.value = ''
      this.input.nativeElement.blur()
      // update the autocomplete options and invalidate the selection
      this.filterStudents()
      this.selectedStudent = null
    }
  }

  removeStudent(student: Student){
    this.data.owners = this.data.owners.filter(d => d.id != student.id)
    this.students.push(student)
    this.filterStudents()
  }

}
