import { Component, Inject, OnInit, ViewChild } from '@angular/core';
import { MatDialog, MatDialogConfig, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { MatPaginator } from '@angular/material/paginator';
import { MatTableDataSource } from '@angular/material/table';
import { Subscription } from 'rxjs';
import { ContentDialogComponent } from './content-dialog.component';
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

  displayedColumns: string[] = ['timestamp', 'type', 'content'];
  dataSource: MatTableDataSource<Work>

  subscriptions: Subscription = new Subscription()

  numbers: number[]
  vote: number;
  laude: boolean;

  constructor(public dialogHistory: MatDialogRef<HistoryDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData, private assignmentService: AssignmentService, private dialog: MatDialog) {
      
      console.log("Sono nel costruttore di history dialog");
      console.log(data.works)
      this.dataSource = new MatTableDataSource(data.works)
      this.dataSource.paginator = this.paginator

      this.numbers = Array.from(Array(31).keys()); 
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

    giveVote() {
      console.log("Sono in giveVote()")
      console.log(this.vote)
      console.log(this.laude)
      this.subscriptions.add(this.assignmentService.giveVote(this.data.assignmentId, this.data.studentId, this.vote, this.laude).subscribe())
    }

    disable() {

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

  ngOnInit(): void {
  }

}
