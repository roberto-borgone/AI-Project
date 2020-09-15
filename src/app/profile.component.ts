import { Component, OnInit } from '@angular/core';
import { ProfileService } from './services/profile.service';
import { Profile } from './profile.model';
import { Subscription } from 'rxjs';


@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css']
})
export class ProfileComponent implements OnInit {

  profile: Profile;
  url: any;
  subscriptions: Subscription = new Subscription();
  uploadFail: boolean = false

  constructor(private profileService : ProfileService) {
    this.profile = new Profile('','','','');
  }

  ngOnInit(): void {
    this.subscriptions.add(this.profileService.getProfile().subscribe(res => this.profile = res));
    this.getImg()
  }

  getImg(){
    this.subscriptions.add(this.profileService.getImg().subscribe((baseImage : any) => {
      var reader = new FileReader();
      reader.readAsDataURL(baseImage);
      reader.onload = (_event) => {
      this.url = reader.result;
      }
    }));
  }

  onClick() { 
    const fileUpload = document.getElementById('imageUpload') as HTMLInputElement;
    fileUpload.click();
}

  handleImageSelect(event: any) {
    var files = event.target.files; // FileList object
    var file = files[0];
    this.subscriptions.add(this.profileService.sendImg(file).subscribe(result => {
      this.getImg()
      this.uploadFail = !result
    }))
  }

  ngOnDestroy(){
    this.subscriptions.unsubscribe();
  }
}
