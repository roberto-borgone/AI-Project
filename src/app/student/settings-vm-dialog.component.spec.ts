import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { SettingsVmDialogComponent } from './settings-vm-dialog.component';

describe('SettingsVmDialogComponent', () => {
  let component: SettingsVmDialogComponent;
  let fixture: ComponentFixture<SettingsVmDialogComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ SettingsVmDialogComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SettingsVmDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
