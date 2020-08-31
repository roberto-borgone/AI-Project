import { Component, OnInit } from '@angular/core';
import { ProfileService } from './services/profile.service';
import { Profile } from './profile.model';


@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css']
})
export class ProfileComponent implements OnInit {

  profile: Profile;

  constructor(private profileService : ProfileService) {
    this.profile = new Profile('','','','');
  }

  ngOnInit(): void {
    this.profileService.getProfile().subscribe(res => this.profile = res);
    this.profileService.getImg().subscribe(res => console.log("Ciaooo"));
  }

  onClick() { 
    const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
    fileUpload.click();
}

  handleImageSelect(event: any) {
    console.log("Sono in handleFileSelect");
    var files = event.target.files; // FileList object
    var file = files[0];
    console.log(file);
    this.profileService.sendImg(file);
    var reader = new FileReader();
    

    reader.readAsText(file);
    reader.onload = (event: any) => {
      var img = event.target.result; // Content of image file
      console.log(img);
    }
  }
}

