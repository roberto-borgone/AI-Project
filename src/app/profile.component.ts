import { Component, OnInit } from '@angular/core';
import { ProfileService } from './services/profile.service';
import { Profile } from './profile.model';
import { DomSanitizer } from '@angular/platform-browser';


@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css']
})
export class ProfileComponent implements OnInit {

  profile: Profile;
  url: any;

  constructor(private profileService : ProfileService, private sanitizer : DomSanitizer) {
    this.profile = new Profile('','','','');
  }

  ngOnInit(): void {
    this.profileService.getProfile().subscribe(res => this.profile = res);
    this.profileService.getImg().subscribe((baseImage : any) => {
      
      var reader = new FileReader();
      reader.readAsDataURL(baseImage);
      reader.onload = (_event) => {
			this.url = reader.result; 
		}
    });
  }

  onClick() { 
    const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
    fileUpload.click();
}

  handleImageSelect(event: any) {
    console.log("Sono in handleImageSelect");
    var files = event.target.files; // FileList object
    var file = files[0];
    this.profileService.sendImg(file);
  }
}

