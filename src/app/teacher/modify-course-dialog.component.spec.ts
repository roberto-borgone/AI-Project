import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModifyCourseDialogComponent } from './modify-course-dialog.component';

describe('ModifyCourseDialogComponent', () => {
  let component: ModifyCourseDialogComponent;
  let fixture: ComponentFixture<ModifyCourseDialogComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModifyCourseDialogComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModifyCourseDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
