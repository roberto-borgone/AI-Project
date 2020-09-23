import { Component, Inject } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Router } from '@angular/router';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';

export interface DialogData {
  maxRAM: FormControl;
  maxDisk: FormControl;
  maxVCPU: FormControl;
  maxTotVM: FormControl;
  maxActiveVM: FormControl;
  updateVMInvalid: boolean;
  placeholders: Placeholders
  message: String;
}

export interface Placeholders {
  maxRAM: number
  maxDisk: number
  maxVCPU: number
  maxTotVM: number
  maxActiveVM: number
}

@Component({
  selector: 'app-update-vm-dialog',
  templateUrl: './update-vm-dialog.component.html',
  styleUrls: ['./update-vm-dialog.component.css']
})
export class UpdateVMDialogComponent {

  message: String

  constructor(private router: Router,
    public dialogRef: MatDialogRef<UpdateVMDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData) {
      this.message = data.message
    }

  onNoClick(): void {
    this.dialogRef.close();
  }

  getErrorMessageMaxRAM(maxRAM: FormControl) {
    return 'Inserisci un valore valido per la RAM'
  }

  getErrorMessageMaxDisk(maxDisk: FormControl) {
    return 'Inserisci un valore valido per il disco';
  }

  getErrorMessageMaxVCPU(maxVCPU: FormControl) {
    return 'Inserisci un valore valido per le VCPU';
  }

  getErrorMessageMaxTotVM(maxTotVM: FormControl) {
    return 'Inserisci un numero valido di VM';
  }

  getErrorMessageMaxActiveVM(maxActiveVM: FormControl) {
    return 'Inserisci un numero valido di VM';
  }

  getMessage(){
    return this.message;
  }

}
