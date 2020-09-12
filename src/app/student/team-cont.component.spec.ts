import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { TeamContComponent } from './team-cont.component';

describe('TeamContComponent', () => {
  let component: TeamContComponent;
  let fixture: ComponentFixture<TeamContComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ TeamContComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TeamContComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
