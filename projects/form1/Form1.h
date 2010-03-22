#pragma once

#include "VstHost/VstHost.h"
#include <sstream>
#include <string>


namespace form1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	private:
		VstHost ^ host;
		int numParams;			//pocet parametrof efektu
		int * selectedParams;	//ktore paramtere su vybrate
		int maxParams;			//maximalny pocet paramterov efektu
		int numSelectedParams;	//pocet vybratych parametrov
	private: System::Windows::Forms::GroupBox^  group_Params;


	private: System::Windows::Forms::CheckBox^  checkBox9;
	private: System::Windows::Forms::CheckBox^  checkBox8;
	private: System::Windows::Forms::CheckBox^  checkBox7;
	private: System::Windows::Forms::CheckBox^  checkBox6;
	private: System::Windows::Forms::CheckBox^  checkBox5;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  b_Process;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::Button^  b_Save;



			 String ^ nl;

	public:

		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			nl = gcnew String(Environment::NewLine);
			host = gcnew VstHost();
			console->AppendText(host->message + nl);
			numParams = 0;
			maxParams = 9;
			numSelectedParams = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			delete host;
		}
	private: System::Windows::Forms::Button^  b_Dry;
	private: System::Windows::Forms::Button^  b_Wet;
	private: System::Windows::Forms::Button^  b_Effect;
	private: System::Windows::Forms::Label^  l_Dry;
	private: System::Windows::Forms::Label^  l_Wet;
	private: System::Windows::Forms::Label^  l_Effect;
	private: System::Windows::Forms::Label^  l_DryFile;
	private: System::Windows::Forms::Label^  l_WetFile;
	private: System::Windows::Forms::Label^  l_EffectFile;
	private: System::Windows::Forms::ComboBox^  combo_Type;
	private: System::Windows::Forms::TextBox^  console;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog3;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->b_Dry = (gcnew System::Windows::Forms::Button());
			this->b_Wet = (gcnew System::Windows::Forms::Button());
			this->b_Effect = (gcnew System::Windows::Forms::Button());
			this->l_Dry = (gcnew System::Windows::Forms::Label());
			this->l_Wet = (gcnew System::Windows::Forms::Label());
			this->l_Effect = (gcnew System::Windows::Forms::Label());
			this->l_DryFile = (gcnew System::Windows::Forms::Label());
			this->l_WetFile = (gcnew System::Windows::Forms::Label());
			this->l_EffectFile = (gcnew System::Windows::Forms::Label());
			this->combo_Type = (gcnew System::Windows::Forms::ComboBox());
			this->console = (gcnew System::Windows::Forms::TextBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog3 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->group_Params = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox9 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox8 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox7 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox6 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->b_Process = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->b_Save = (gcnew System::Windows::Forms::Button());
			this->group_Params->SuspendLayout();
			this->SuspendLayout();
			// 
			// b_Dry
			// 
			this->b_Dry->Location = System::Drawing::Point(12, 12);
			this->b_Dry->Name = L"b_Dry";
			this->b_Dry->Size = System::Drawing::Size(85, 23);
			this->b_Dry->TabIndex = 0;
			this->b_Dry->Text = L"Select Dry";
			this->b_Dry->UseVisualStyleBackColor = true;
			this->b_Dry->Click += gcnew System::EventHandler(this, &Form1::b_Dry_Click);
			// 
			// b_Wet
			// 
			this->b_Wet->Location = System::Drawing::Point(12, 41);
			this->b_Wet->Name = L"b_Wet";
			this->b_Wet->Size = System::Drawing::Size(85, 23);
			this->b_Wet->TabIndex = 1;
			this->b_Wet->Text = L"Select Wet";
			this->b_Wet->UseVisualStyleBackColor = true;
			this->b_Wet->Click += gcnew System::EventHandler(this, &Form1::b_Wet_Click);
			// 
			// b_Effect
			// 
			this->b_Effect->Location = System::Drawing::Point(12, 70);
			this->b_Effect->Name = L"b_Effect";
			this->b_Effect->Size = System::Drawing::Size(85, 23);
			this->b_Effect->TabIndex = 2;
			this->b_Effect->Text = L"Select Effect";
			this->b_Effect->UseVisualStyleBackColor = true;
			this->b_Effect->Click += gcnew System::EventHandler(this, &Form1::b_Effect_Click);
			// 
			// l_Dry
			// 
			this->l_Dry->AutoSize = true;
			this->l_Dry->Location = System::Drawing::Point(103, 17);
			this->l_Dry->Name = L"l_Dry";
			this->l_Dry->Size = System::Drawing::Size(26, 13);
			this->l_Dry->TabIndex = 3;
			this->l_Dry->Text = L"Dry:";
			// 
			// l_Wet
			// 
			this->l_Wet->AutoSize = true;
			this->l_Wet->Location = System::Drawing::Point(103, 46);
			this->l_Wet->Name = L"l_Wet";
			this->l_Wet->Size = System::Drawing::Size(27, 13);
			this->l_Wet->TabIndex = 4;
			this->l_Wet->Text = L"Wet";
			// 
			// l_Effect
			// 
			this->l_Effect->AutoSize = true;
			this->l_Effect->Location = System::Drawing::Point(103, 75);
			this->l_Effect->Name = L"l_Effect";
			this->l_Effect->Size = System::Drawing::Size(38, 13);
			this->l_Effect->TabIndex = 5;
			this->l_Effect->Text = L"Effect:";
			// 
			// l_DryFile
			// 
			this->l_DryFile->AutoSize = true;
			this->l_DryFile->Location = System::Drawing::Point(147, 17);
			this->l_DryFile->Name = L"l_DryFile";
			this->l_DryFile->Size = System::Drawing::Size(63, 13);
			this->l_DryFile->TabIndex = 6;
			this->l_DryFile->Text = L"- select file -";
			// 
			// l_WetFile
			// 
			this->l_WetFile->AutoSize = true;
			this->l_WetFile->Location = System::Drawing::Point(147, 46);
			this->l_WetFile->Name = L"l_WetFile";
			this->l_WetFile->Size = System::Drawing::Size(63, 13);
			this->l_WetFile->TabIndex = 7;
			this->l_WetFile->Text = L"- select file -";
			// 
			// l_EffectFile
			// 
			this->l_EffectFile->AutoSize = true;
			this->l_EffectFile->Location = System::Drawing::Point(147, 75);
			this->l_EffectFile->Name = L"l_EffectFile";
			this->l_EffectFile->Size = System::Drawing::Size(63, 13);
			this->l_EffectFile->TabIndex = 8;
			this->l_EffectFile->Text = L"- select file -";
			// 
			// combo_Type
			// 
			this->combo_Type->FormattingEnabled = true;
			this->combo_Type->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"volume", L"delay", L"flanger", L"compressor", 
				L"tremolo"});
			this->combo_Type->Location = System::Drawing::Point(12, 100);
			this->combo_Type->Name = L"combo_Type";
			this->combo_Type->Size = System::Drawing::Size(121, 21);
			this->combo_Type->TabIndex = 9;
			this->combo_Type->Text = L"- select effect type -";
			// 
			// console
			// 
			this->console->BackColor = System::Drawing::SystemColors::Window;
			this->console->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->console->ForeColor = System::Drawing::SystemColors::WindowText;
			this->console->Location = System::Drawing::Point(12, 271);
			this->console->Multiline = true;
			this->console->Name = L"console";
			this->console->ReadOnly = true;
			this->console->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->console->Size = System::Drawing::Size(476, 183);
			this->console->TabIndex = 10;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"d:\\Skola\\rocnikac\\solution\\projects\\form1\\cymbal3.wav";
			this->openFileDialog1->Filter = L"Wave files (*.wav)|*.wav|All files (*.*)|*.*";
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"d:\\Skola\\rocnikac\\solution\\projects\\form1\\cymbal3_-6dB.wav";
			this->openFileDialog2->Filter = L"Wave files (*.wav)|*.wav|All files (*.*)|*.*";
			// 
			// openFileDialog3
			// 
			this->openFileDialog3->FileName = L"d:\\Skola\\rocnikac\\solution\\projects\\form1\\doubledelay.dll";
			this->openFileDialog3->Filter = L"dll files (*.dll)|*.dll|All files (*.*)|*.*";
			// 
			// group_Params
			// 
			this->group_Params->Controls->Add(this->checkBox9);
			this->group_Params->Controls->Add(this->checkBox8);
			this->group_Params->Controls->Add(this->checkBox7);
			this->group_Params->Controls->Add(this->checkBox6);
			this->group_Params->Controls->Add(this->checkBox5);
			this->group_Params->Controls->Add(this->checkBox4);
			this->group_Params->Controls->Add(this->checkBox3);
			this->group_Params->Controls->Add(this->checkBox2);
			this->group_Params->Controls->Add(this->checkBox1);
			this->group_Params->Location = System::Drawing::Point(12, 127);
			this->group_Params->Name = L"group_Params";
			this->group_Params->Size = System::Drawing::Size(338, 93);
			this->group_Params->TabIndex = 11;
			this->group_Params->TabStop = false;
			this->group_Params->Text = L"Parameters to process";
			// 
			// checkBox9
			// 
			this->checkBox9->AutoSize = true;
			this->checkBox9->Checked = true;
			this->checkBox9->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox9->Location = System::Drawing::Point(17, 19);
			this->checkBox9->Name = L"checkBox9";
			this->checkBox9->Size = System::Drawing::Size(80, 17);
			this->checkBox9->TabIndex = 8;
			this->checkBox9->Text = L"checkBox9";
			this->checkBox9->UseVisualStyleBackColor = true;
			this->checkBox9->Visible = false;
			// 
			// checkBox8
			// 
			this->checkBox8->AutoSize = true;
			this->checkBox8->Checked = true;
			this->checkBox8->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox8->Location = System::Drawing::Point(17, 43);
			this->checkBox8->Name = L"checkBox8";
			this->checkBox8->Size = System::Drawing::Size(80, 17);
			this->checkBox8->TabIndex = 7;
			this->checkBox8->Text = L"checkBox8";
			this->checkBox8->UseVisualStyleBackColor = true;
			this->checkBox8->Visible = false;
			// 
			// checkBox7
			// 
			this->checkBox7->AutoSize = true;
			this->checkBox7->Checked = true;
			this->checkBox7->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox7->Location = System::Drawing::Point(17, 66);
			this->checkBox7->Name = L"checkBox7";
			this->checkBox7->Size = System::Drawing::Size(80, 17);
			this->checkBox7->TabIndex = 6;
			this->checkBox7->Text = L"checkBox7";
			this->checkBox7->UseVisualStyleBackColor = true;
			this->checkBox7->Visible = false;
			// 
			// checkBox6
			// 
			this->checkBox6->AutoSize = true;
			this->checkBox6->Checked = true;
			this->checkBox6->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox6->Location = System::Drawing::Point(127, 19);
			this->checkBox6->Name = L"checkBox6";
			this->checkBox6->Size = System::Drawing::Size(80, 17);
			this->checkBox6->TabIndex = 5;
			this->checkBox6->Text = L"checkBox6";
			this->checkBox6->UseVisualStyleBackColor = true;
			this->checkBox6->Visible = false;
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Checked = true;
			this->checkBox5->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox5->Location = System::Drawing::Point(127, 43);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(80, 17);
			this->checkBox5->TabIndex = 4;
			this->checkBox5->Text = L"checkBox5";
			this->checkBox5->UseVisualStyleBackColor = true;
			this->checkBox5->Visible = false;
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Checked = true;
			this->checkBox4->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox4->Location = System::Drawing::Point(127, 66);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(80, 17);
			this->checkBox4->TabIndex = 3;
			this->checkBox4->Text = L"checkBox4";
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->Visible = false;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Checked = true;
			this->checkBox3->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox3->Location = System::Drawing::Point(237, 19);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(80, 17);
			this->checkBox3->TabIndex = 2;
			this->checkBox3->Text = L"checkBox3";
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->Visible = false;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Checked = true;
			this->checkBox2->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox2->Location = System::Drawing::Point(237, 43);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(80, 17);
			this->checkBox2->TabIndex = 1;
			this->checkBox2->Text = L"checkBox2";
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->Visible = false;
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Checked = true;
			this->checkBox1->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox1->Location = System::Drawing::Point(237, 66);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(80, 17);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"checkBox1";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->Visible = false;
			// 
			// b_Process
			// 
			this->b_Process->Location = System::Drawing::Point(356, 128);
			this->b_Process->Name = L"b_Process";
			this->b_Process->Size = System::Drawing::Size(130, 93);
			this->b_Process->TabIndex = 12;
			this->b_Process->Text = L"PROCESS";
			this->b_Process->UseVisualStyleBackColor = true;
			this->b_Process->Click += gcnew System::EventHandler(this, &Form1::b_Process_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(12, 227);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(338, 38);
			this->progressBar1->TabIndex = 13;
			this->progressBar1->Text = L"asdasd";
			this->progressBar1->UseWaitCursor = true;
			// 
			// b_Save
			// 
			this->b_Save->Location = System::Drawing::Point(356, 227);
			this->b_Save->Name = L"b_Save";
			this->b_Save->Size = System::Drawing::Size(130, 38);
			this->b_Save->TabIndex = 14;
			this->b_Save->Text = L"SAVE";
			this->b_Save->UseVisualStyleBackColor = true;
			this->b_Save->Click += gcnew System::EventHandler(this, &Form1::b_Save_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(498, 466);
			this->Controls->Add(this->b_Save);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->b_Process);
			this->Controls->Add(this->group_Params);
			this->Controls->Add(this->console);
			this->Controls->Add(this->combo_Type);
			this->Controls->Add(this->l_EffectFile);
			this->Controls->Add(this->l_WetFile);
			this->Controls->Add(this->l_DryFile);
			this->Controls->Add(this->l_Effect);
			this->Controls->Add(this->l_Wet);
			this->Controls->Add(this->l_Dry);
			this->Controls->Add(this->b_Effect);
			this->Controls->Add(this->b_Wet);
			this->Controls->Add(this->b_Dry);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->group_Params->ResumeLayout(false);
			this->group_Params->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void b_Dry_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					l_DryFile->Text = openFileDialog1->FileName;
					host->openDryFile(openFileDialog1->FileName);
					console->AppendText(host->message + nl);
				 }
			 }
private: System::Void b_Wet_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (openFileDialog2->ShowDialog() == System::Windows::Forms::DialogResult::OK){
					l_WetFile->Text = openFileDialog2->FileName;
					host->openWetFile(openFileDialog2->FileName);
					console->AppendText(host->message + nl);
			 }
		 }
private: System::Void b_Effect_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (openFileDialog3->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				//nacitame efekt
				l_EffectFile->Text = openFileDialog3->FileName;
				try {
					host->openEffectFile(openFileDialog3->FileName);
				}
				catch (Exception ^ e) {
					console->AppendText(e->Message + nl);
				}
				if (e->Empty) {
					console->AppendText(host->message + nl);
					
					//zistime paramtere
					numParams = host->getNumParams();
					
					if (numParams > maxParams) {
						console->AppendText(gcnew String("ERROR: More than 9 parameters in effect!") + nl);
					} 
					else {
						
						//skryjeme vsetky checkboxy
						for (int i=0; i<maxParams; i++) {
							group_Params->Controls[i]->Visible = false;
						}
						//ukazeme niektore checkboxy
						for (int i=0; i<numParams; i++) {
							group_Params->Controls[i]->Text = host->getParamName(i);
							group_Params->Controls[i]->Visible = true;
						}
					}
				}
			 }
		 }

private: System::Void b_Process_Click(System::Object^  sender, System::EventArgs^  e) {
			if (host->getOk() && (combo_Type->SelectedIndex!=-1)) {	//ak su zvolene vsetky parametre programu
				 selectedParams = new int[numParams];
				 //zistime selectedParams a numSelectedParams
				 for (int i=0; i<numParams; i++) {
					 System::Windows::Forms::CheckBox^ c = (System::Windows::Forms::CheckBox^)group_Params->Controls[i];
					 if (c->Checked) {
						 selectedParams[numSelectedParams++] = i;
					 }
				 }
				 //spustime process
				 host->process(combo_Type->SelectedIndex, selectedParams, numSelectedParams, progressBar1);
				 console->AppendText(host->message + nl);
			 }
			 else {
				 console->AppendText(gcnew String("ERROR: some of program's parameters not chosen!") + nl);
			 }
		 }
private: System::Void b_Save_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (!host->getProcessed()) {
				 console->AppendText(gcnew String("ERROR: Parameter's values not processed, cannot save!" + nl));
			 }
			 else if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 try {
						 host->saveProgram(saveFileDialog1->FileName);
					 }
					 catch (Exception^ e) {
					 }
					 console->AppendText(host->message + nl);
			 }
		 }
};
}

