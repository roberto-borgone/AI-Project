import { Component, OnInit } from '@angular/core';
import { Profile } from './profile.model';
import { ProfileService } from './services/profile.service';


@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css']
})
export class ProfileComponent implements OnInit {

  profile: Profile;

  constructor(private service: ProfileService) {}

  ngOnInit(): void {
    this.service.getProfile()
  }

  onClick() { 
    const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
    fileUpload.click();
}

  handleImageSelect(event: any) {
    console.log("Sono in handleFileSelect");
    var files = event.target.files; // FileList object
    var file = files[0];
    var reader = new FileReader();

    reader.readAsText(file);
    reader.onload = (event: any) => {
      var img = event.target.result; // Content of image file
      console.log(img);
    }
  }
}
