#include "sampleModule.h"
#include <gtkmm.h>
#include <iostream>

SampleModule::~SampleModule() {

}

/* Creates custom widgets (like buttons) for the custom module.
This basically expands upon the parent Module. */
void SampleModule::populateModule()
{
    std::cout << "Start populating custom Module" << std::endl;

    // Create a button, with label "Press Me"
    this->button = Gtk::Button("Button");

    // When the button receives the "clicked" signal, it will call the
    // on_button_clicked() method defined below.
    this->button.signal_clicked().connect(sigc::mem_fun(*this,
                                                        &SampleModule::on_button_clicked));

    // Add Button to our Box (the Box holds all the widgets of this Module)
    // Shrink Widget to its size, add 0 padding
    this->box.pack_start(button, Gtk::PACK_SHRINK,0);

    // Create a second button to demonstrate how multiple widgets can be added to Box
    this->button2 = Gtk::Button("Press Me 2");
    this->button2.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &SampleModule::on_button_clicked));
    this->box.pack_start(button2);

    std::cout << "Finished populating custom Module" << std::endl;
}

void SampleModule::on_button_clicked()
{
    std::cout << "Button was pressed." << std::endl;
}