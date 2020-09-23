import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { OpenVmDialogComponent } from './open-vm-dialog.component';

describe('OpenVmDialogComponent', () => {
  let component: OpenVmDialogComponent;
  let fixture: ComponentFixture<OpenVmDialogComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ OpenVmDialogComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(OpenVmDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
