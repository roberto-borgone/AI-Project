import { Component, Inject, OnInit, ViewChild } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { Subscription } from 'rxjs';
import { AssignmentService } from './services/assignment.service';
import { Work } from './work.model';

export interface DialogData {
  works: Work[];
  assignmentId: number;
  studentId: string;
}

@Component({
  selector: 'app-history-dialog',
  templateUrl: './history-dialog.component.html',
  styleUrls: ['./history-dialog.component.css']
})
export class HistoryDialogComponent implements OnInit {

  @ViewChild(MatPaginator) 
  paginator: MatPaginator

  displayedColumns: string[] = ['timestamp', 'type', 'vote', 'laude'];
  dataSource: MatTableDataSource<Work>

  subscriptions: Subscription = new Subscription()

  constructor(public dialogRef: MatDialogRef<HistoryDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData, private assignmentService: AssignmentService) {
      
      console.log("Sono nel costruttore di history dialog");
      console.log(data.works)
      this.dataSource = new MatTableDataSource(data.works)
      this.dataSource.paginator = this.paginator
     }


     onClick() { 
      const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
      fileUpload.click();
    }
  
    handleImageSelect(event: any) {
      console.log("Sono in handleImageSelect");
      var files = event.target.files; // FileList object
      var file = files[0];
      this.subscriptions.add(this.assignmentService.uploadCorrection(this.data.assignmentId, this.data.studentId, file).subscribe())
      }

  ngOnInit(): void {
  }

}
