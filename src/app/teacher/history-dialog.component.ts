import { Component, Inject, OnInit, ViewChild } from '@angular/core';
import { MatDialog, MatDialogConfig, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { Subscription } from 'rxjs';
import { ContentDialogComponent } from '../content-dialog.component';
import { LastWork } from '../models/last-work.model';
import { Work } from '../models/work.model';
import { AssignmentService } from '../services/assignment.service';

export interface DialogData {
  works: Work[];
  lastWork: LastWork
}

@Component({
  selector: 'app-history-dialog',
  templateUrl: './history-dialog.component.html',
  styleUrls: ['./history-dialog.component.css']
})
export class HistoryDialogComponent {

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  displayedColumns: string[] = ['timestamp', 'type', 'content'];
  dataSource: MatTableDataSource<Work>

  subscriptions: Subscription = new Subscription()

  numbers: number[]
  vote: number;
  laude: boolean;
  isDisabledLode: boolean;
  isDisabledVoto: boolean;
  buttonDisable: boolean;
  isDisabledSetVote: boolean;
  isDisabledFileUpload: boolean;
  isButtonDisabledOn: boolean;
  isButtonDisabledOff: boolean;

  constructor(public dialogHistory: MatDialogRef<HistoryDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData, private assignmentService: AssignmentService, private dialog: MatDialog) {
      
      console.log("Sono nel costruttore di history dialog");
      console.log(data.works)
      this.dataSource = new MatTableDataSource(data.works)
      this.dataSource.paginator = this.paginator
      this.numbers = Array.from(Array(31).keys()); 
      this.isDisabledLode = true;

      if(this.data.works.length==0 || (this.data.lastWork.voto != undefined && this.data.lastWork.voto != 0 && this.data.lastWork.voto != -1)) {
        this.buttonDisable = true;  
        this.isDisabledVoto = true;
        this.isDisabledSetVote = true;
        this.isDisabledFileUpload = true;
        this.isButtonDisabledOff = true;
        this.isButtonDisabledOn = true;
      }

      

      if(!this.data.lastWork.updateable) {
        this.buttonDisable = true; 
      }
     }

     onClick() { 
      const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
      fileUpload.click();
    }

    onNoClick(): void {
      this.dialogHistory.close();
    }
  
    handleImageSelect(event: any) {
      console.log("Sono in handleImageSelect");
      var files = event.target.files; // FileList object
      var file = files[0];
      this.subscriptions.add(this.assignmentService.uploadCorrection(this.data.lastWork.consegnaId, this.data.lastWork.studentId, file).subscribe(res =>
        this.assignmentService.getStudentHistory(this.data.lastWork.consegnaId, this.data.lastWork.studentId).subscribe(history => this.dataSource = new MatTableDataSource(history))
      ))
    }

    onClickVote() {
      console.log("Sono in onClickVote()")
      console.log(this.vote)
      if(this.vote == 30) {
        this.isDisabledLode = false;
      }
      else {
        this.isDisabledLode = true;
      }
    }

    setVote() {
      console.log("Sono in giveVote()")
      console.log(this.vote)
      console.log(this.laude)

      if(this.laude === undefined) {
        this.laude = false;
      }
      if(this.vote>1) {
        this.subscriptions.add(this.assignmentService.setVote(this.data.lastWork.consegnaId, this.data.lastWork.studentId, this.vote, this.laude)
        .subscribe(res => {
          if(res) {
            this.buttonDisable = true;
            this.isDisabledVoto = true;
            this.isDisabledSetVote = true;
            this.isDisabledLode = true;
            this.isDisabledFileUpload = true;
            this.isButtonDisabledOff = true;
            this.isButtonDisabledOn = true;
            //this.assignmentService.getStudentHistory(this.data.lastWork.consegnaId, this.data.lastWork.studentId).subscribe(history => this.dataSource = new MatTableDataSource(history))
            this.dialogHistory.close();
          }
        }))
      }
    }

    disable() {
      this.subscriptions.add(this.assignmentService.disableAssignment(this.data.lastWork.consegnaId, this.data.lastWork.studentId)
      .subscribe(res => {if(res){
        if(this.buttonDisable === true)
          this.buttonDisable = false
        else 
          this.buttonDisable = true
      }
    }))
  }

    showContent(workId: number) {
      this.subscriptions.add(this.assignmentService.getWorkContent(workId).subscribe(img => {

        const dialogConfig = new MatDialogConfig();
        dialogConfig.width = '80%';
        dialogConfig.height = '70%';
        dialogConfig.data = img;
    
        let dialogRef = this.dialog.open(ContentDialogComponent, dialogConfig);
    
        this.subscriptions.add(dialogRef.afterClosed().subscribe());
      }))
    }

    ngOnDestroy(): void {
      this.subscriptions.unsubscribe();
    }

}
