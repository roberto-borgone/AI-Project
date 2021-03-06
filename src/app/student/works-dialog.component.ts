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
  worksData: Work[];
  lastWorkData: LastWork[];
}

@Component({
  selector: 'app-works-dialog',
  templateUrl: './works-dialog.component.html',
  styleUrls: ['./works-dialog.component.css']
})
export class WorksDialogComponent {

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  displayedColumns2: string[] = ['timestamp', 'type', 'content'];
  displayedColumns1: string[] = ['id', 'name', 'surname', 'status', 'timestamp', 'voto', 'lode'];

  works: MatTableDataSource<Work>
  lastWork: MatTableDataSource<LastWork>

  subscriptions: Subscription = new Subscription()

  isDisabledFileUpload: boolean;

  error: string;

  testoButton: String = "Carica elaborato"

  constructor(public dialogHistory: MatDialogRef<WorksDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData, 
    private assignmentService: AssignmentService, 
    private dialog: MatDialog) {
      
      this.works = new MatTableDataSource(data.worksData)
      this.works.paginator = this.paginator;
      this.lastWork = new MatTableDataSource(data.lastWorkData)

      if(data.lastWorkData[0].updateable == false || data.lastWorkData[0].status == 'CONSEGNATO'|| (data.lastWorkData[0].status != 'RIVISTO' && data.lastWorkData[0].status != 'LETTO')) {
        this.isDisabledFileUpload = true;
        this.testoButton = "Consegna disabilitata"
      }

      if(data.lastWorkData[0].status=='NULL') {
        this.error = "Leggere il testo della consegna per poter caricare un elaborato"
      }

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

  onClick() { 
    const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
    fileUpload.click();
  }

  handleImageSelect(event: any) {
    console.log("Sono in handleImageSelect");
    var files = event.target.files; // FileList object
    var file = files[0];
    this.subscriptions.add(this.assignmentService.uploadWork(this.data.lastWorkData[0].consegnaId, file).subscribe(res => {
      this.subscriptions.add(this.assignmentService.getStudentWorksStudent(this.data.lastWorkData[0].consegnaId).subscribe(worksData => {
        console.log(worksData)
        this.works = new MatTableDataSource(worksData);
        this.subscriptions.add(this.assignmentService.getStudentStatus(this.data.lastWorkData[0].consegnaId).subscribe(res => {
          let lastWorkArray : LastWork[] = [];
          lastWorkArray.push(res);
          console.log(lastWorkArray);
          this.lastWork = new MatTableDataSource(lastWorkArray);
          this.isDisabledFileUpload = true;
          this.testoButton = "Consegna disabilitata"
        }))
      }))
    }));
  }

  onNoClick(): void {
    this.dialogHistory.close();
  }
}
