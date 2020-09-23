import { Component, Inject, OnInit } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { DomSanitizer } from '@angular/platform-browser';

@Component({
  selector: 'app-open-vm-dialog',
  templateUrl: './open-vm-dialog.component.html',
  styleUrls: ['./open-vm-dialog.component.css']
})
export class OpenVmDialogComponent implements OnInit {

  imageSource: any;

  constructor(private sanitizer: DomSanitizer, public dialogRef: MatDialogRef<OpenVmDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any) {}

  ngOnInit () {
    this.imageSource = this.sanitizer.bypassSecurityTrustResourceUrl(`data:image/png;base64, ${this.data}`);
  }

  onNoClick(): void {
    this.dialogRef.close();
  }
}
