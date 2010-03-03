#pragma once

#include "VstHost/VstHost.h"


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
			this->combo_Type->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"delay", L"compressor", L"flanger"});
			this->combo_Type->Location = System::Drawing::Point(13, 100);
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
			this->console->Location = System::Drawing::Point(13, 207);
			this->console->Multiline = true;
			this->console->Name = L"console";
			this->console->ReadOnly = true;
			this->console->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->console->Size = System::Drawing::Size(473, 144);
			this->console->TabIndex = 10;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->Filter = L"Wave files (*.wav)|*.wav|All files (*.*)|*.*";
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			this->openFileDialog2->Filter = L"Wave files (*.wav)|*.wav|All files (*.*)|*.*";
			// 
			// openFileDialog3
			// 
			this->openFileDialog3->FileName = L"openFileDialog3";
			this->openFileDialog3->Filter = L"dll files (*.dll)|*.dll|All files (*.*)|*.*";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(498, 363);
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
				l_EffectFile->Text = openFileDialog3->FileName;
				try {
					host->openEffectFile(openFileDialog3->FileName);
				}
				catch (Exception ^ e) {
					console->AppendText("VstHost > " + e->Message + nl);
				}
				if (e->Empty) {
					console->AppendText(host->message + nl);
				}
			 }
		 }
};
}

