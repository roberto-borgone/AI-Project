import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { CoursesContComponent } from './courses-cont.component';

describe('CoursesContComponent', () => {
  let component: CoursesContComponent;
  let fixture: ComponentFixture<CoursesContComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ CoursesContComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(CoursesContComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
