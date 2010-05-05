#pragma once

#include "VstHost/VstHost.h"
#include <sstream>
#include <string>


namespace reSounder {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ReSounder
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ReSounder : public System::Windows::Forms::Form
	{
	private:
		VstHost ^ host;
		int numParams;			//pocet parametrof efektu
		int maxParams;			//maximalny pocet paramterov efektu
		int numSelectedParams;	//pocet vybratych parametrov
		String^ homeDir;		//vchodzi adresar
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





	private: System::Windows::Forms::CheckBox^  checkBox11;
	private: System::Windows::Forms::CheckBox^  checkBox10;
	private: System::Windows::Forms::CheckBox^  checkBox18;
	private: System::Windows::Forms::CheckBox^  checkBox17;
	private: System::Windows::Forms::CheckBox^  checkBox16;
	private: System::Windows::Forms::CheckBox^  checkBox15;
	private: System::Windows::Forms::CheckBox^  checkBox14;
	private: System::Windows::Forms::CheckBox^  checkBox13;
	private: System::Windows::Forms::CheckBox^  checkBox12;
	private: System::Windows::Forms::TableLayoutPanel^  table_Params;




	private: System::Windows::Forms::Panel^  panel_Vol;
	private: System::Windows::Forms::Panel^  panel_Name;


	private: System::Windows::Forms::Panel^  panel_Step;

	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  check_Fast;






			 String ^ nl;

	public:

		ReSounder(void)
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
			homeDir = Directory::GetCurrentDirectory();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ReSounder()
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
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->table_Params = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panel_Name = (gcnew System::Windows::Forms::Panel());
			this->checkBox9 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox8 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox7 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox6 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->panel_Step = (gcnew System::Windows::Forms::Panel());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->panel_Vol = (gcnew System::Windows::Forms::Panel());
			this->checkBox18 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox17 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox16 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox15 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox14 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox13 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox12 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox11 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox10 = (gcnew System::Windows::Forms::CheckBox());
			this->b_Process = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->b_Save = (gcnew System::Windows::Forms::Button());
			this->check_Fast = (gcnew System::Windows::Forms::CheckBox());
			this->group_Params->SuspendLayout();
			this->table_Params->SuspendLayout();
			this->panel_Name->SuspendLayout();
			this->panel_Step->SuspendLayout();
			this->panel_Vol->SuspendLayout();
			this->SuspendLayout();
			// 
			// b_Dry
			// 
			this->b_Dry->Location = System::Drawing::Point(12, 12);
			this->b_Dry->Name = L"b_Dry";
			this->b_Dry->Size = System::Drawing::Size(102, 23);
			this->b_Dry->TabIndex = 0;
			this->b_Dry->Text = L"Select Clean";
			this->b_Dry->UseVisualStyleBackColor = true;
			this->b_Dry->Click += gcnew System::EventHandler(this, &ReSounder::b_Dry_Click);
			// 
			// b_Wet
			// 
			this->b_Wet->Location = System::Drawing::Point(12, 41);
			this->b_Wet->Name = L"b_Wet";
			this->b_Wet->Size = System::Drawing::Size(102, 23);
			this->b_Wet->TabIndex = 1;
			this->b_Wet->Text = L"Select Wanted";
			this->b_Wet->UseVisualStyleBackColor = true;
			this->b_Wet->Click += gcnew System::EventHandler(this, &ReSounder::b_Wet_Click);
			// 
			// b_Effect
			// 
			this->b_Effect->Location = System::Drawing::Point(12, 70);
			this->b_Effect->Name = L"b_Effect";
			this->b_Effect->Size = System::Drawing::Size(102, 23);
			this->b_Effect->TabIndex = 2;
			this->b_Effect->Text = L"Select Effect";
			this->b_Effect->UseVisualStyleBackColor = true;
			this->b_Effect->Visible = false;
			this->b_Effect->Click += gcnew System::EventHandler(this, &ReSounder::b_Effect_Click);
			// 
			// l_Dry
			// 
			this->l_Dry->AutoSize = true;
			this->l_Dry->Location = System::Drawing::Point(125, 17);
			this->l_Dry->Name = L"l_Dry";
			this->l_Dry->Size = System::Drawing::Size(37, 13);
			this->l_Dry->TabIndex = 3;
			this->l_Dry->Text = L"Clean:";
			// 
			// l_Wet
			// 
			this->l_Wet->AutoSize = true;
			this->l_Wet->Location = System::Drawing::Point(125, 46);
			this->l_Wet->Name = L"l_Wet";
			this->l_Wet->Size = System::Drawing::Size(48, 13);
			this->l_Wet->TabIndex = 4;
			this->l_Wet->Text = L"Wanted:";
			this->l_Wet->Click += gcnew System::EventHandler(this, &ReSounder::l_Wet_Click);
			// 
			// l_Effect
			// 
			this->l_Effect->AutoSize = true;
			this->l_Effect->Location = System::Drawing::Point(125, 75);
			this->l_Effect->Name = L"l_Effect";
			this->l_Effect->Size = System::Drawing::Size(38, 13);
			this->l_Effect->TabIndex = 5;
			this->l_Effect->Text = L"Effect:";
			this->l_Effect->Visible = false;
			// 
			// l_DryFile
			// 
			this->l_DryFile->AutoSize = true;
			this->l_DryFile->Location = System::Drawing::Point(179, 17);
			this->l_DryFile->Name = L"l_DryFile";
			this->l_DryFile->Size = System::Drawing::Size(63, 13);
			this->l_DryFile->TabIndex = 6;
			this->l_DryFile->Text = L"- select file -";
			// 
			// l_WetFile
			// 
			this->l_WetFile->AutoSize = true;
			this->l_WetFile->Location = System::Drawing::Point(179, 46);
			this->l_WetFile->Name = L"l_WetFile";
			this->l_WetFile->Size = System::Drawing::Size(63, 13);
			this->l_WetFile->TabIndex = 7;
			this->l_WetFile->Text = L"- select file -";
			// 
			// l_EffectFile
			// 
			this->l_EffectFile->AutoSize = true;
			this->l_EffectFile->Location = System::Drawing::Point(179, 75);
			this->l_EffectFile->Name = L"l_EffectFile";
			this->l_EffectFile->Size = System::Drawing::Size(63, 13);
			this->l_EffectFile->TabIndex = 8;
			this->l_EffectFile->Text = L"- select file -";
			this->l_EffectFile->Visible = false;
			// 
			// combo_Type
			// 
			this->combo_Type->Cursor = System::Windows::Forms::Cursors::Default;
			this->combo_Type->FormattingEnabled = true;
			this->combo_Type->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"general", L"single", L"delay", L"equalizer", 
				L"limiter", L"compressor", L"pitch shifter"});
			this->combo_Type->Location = System::Drawing::Point(12, 100);
			this->combo_Type->Name = L"combo_Type";
			this->combo_Type->Size = System::Drawing::Size(121, 21);
			this->combo_Type->TabIndex = 9;
			this->combo_Type->Text = L"- select effect type -";
			this->combo_Type->SelectedIndexChanged += gcnew System::EventHandler(this, &ReSounder::combo_Type_SelectedIndexChanged);
			// 
			// console
			// 
			this->console->BackColor = System::Drawing::SystemColors::Window;
			this->console->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->console->ForeColor = System::Drawing::SystemColors::WindowText;
			this->console->Location = System::Drawing::Point(198, 225);
			this->console->Multiline = true;
			this->console->Name = L"console";
			this->console->ReadOnly = true;
			this->console->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->console->Size = System::Drawing::Size(505, 189);
			this->console->TabIndex = 10;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"d:\\Skola\\rocnikac\\solution\\projects\\reSounder\\cymbal3.wav";
			this->openFileDialog1->Filter = L"Wave files (*.wav)|*.wav|All files (*.*)|*.*";
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"d:\\Skola\\rocnikac\\solution\\projects\\reSounder\\cymbal3_-6dB.wav";
			this->openFileDialog2->Filter = L"Wave files (*.wav)|*.wav|All files (*.*)|*.*";
			// 
			// openFileDialog3
			// 
			this->openFileDialog3->Filter = L"dll files (*.dll)|*.dll|All files (*.*)|*.*";
			// 
			// group_Params
			// 
			this->group_Params->Controls->Add(this->label4);
			this->group_Params->Controls->Add(this->label3);
			this->group_Params->Controls->Add(this->label2);
			this->group_Params->Controls->Add(this->label1);
			this->group_Params->Controls->Add(this->table_Params);
			this->group_Params->Location = System::Drawing::Point(12, 127);
			this->group_Params->Name = L"group_Params";
			this->group_Params->Size = System::Drawing::Size(180, 287);
			this->group_Params->TabIndex = 11;
			this->group_Params->TabStop = false;
			this->group_Params->Text = L"Parameters to process";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(9, 20);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(22, 13);
			this->label4->TabIndex = 17;
			this->label4->Text = L"Sel";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(124, 20);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(46, 13);
			this->label3->TabIndex = 16;
			this->label3->Text = L"Step (%)";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(99, 20);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(22, 13);
			this->label2->TabIndex = 15;
			this->label2->Text = L"Vol";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(37, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 14;
			this->label1->Text = L"Name";
			// 
			// table_Params
			// 
			this->table_Params->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->table_Params->ColumnCount = 3;
			this->table_Params->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->table_Params->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				25)));
			this->table_Params->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				46)));
			this->table_Params->Controls->Add(this->panel_Name, 0, 0);
			this->table_Params->Controls->Add(this->panel_Step, 2, 0);
			this->table_Params->Controls->Add(this->panel_Vol, 1, 0);
			this->table_Params->Location = System::Drawing::Point(6, 36);
			this->table_Params->Name = L"table_Params";
			this->table_Params->RowCount = 1;
			this->table_Params->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->table_Params->Size = System::Drawing::Size(163, 245);
			this->table_Params->TabIndex = 13;
			// 
			// panel_Name
			// 
			this->panel_Name->Controls->Add(this->checkBox9);
			this->panel_Name->Controls->Add(this->checkBox8);
			this->panel_Name->Controls->Add(this->checkBox7);
			this->panel_Name->Controls->Add(this->checkBox6);
			this->panel_Name->Controls->Add(this->checkBox5);
			this->panel_Name->Controls->Add(this->checkBox4);
			this->panel_Name->Controls->Add(this->checkBox3);
			this->panel_Name->Controls->Add(this->checkBox2);
			this->panel_Name->Controls->Add(this->checkBox1);
			this->panel_Name->Location = System::Drawing::Point(4, 4);
			this->panel_Name->Name = L"panel_Name";
			this->panel_Name->Size = System::Drawing::Size(82, 237);
			this->panel_Name->TabIndex = 1;
			// 
			// checkBox9
			// 
			this->checkBox9->AutoSize = true;
			this->checkBox9->Checked = true;
			this->checkBox9->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox9->Location = System::Drawing::Point(3, 6);
			this->checkBox9->Name = L"checkBox9";
			this->checkBox9->Size = System::Drawing::Size(80, 17);
			this->checkBox9->TabIndex = 8;
			this->checkBox9->Text = L"checkBox9";
			this->checkBox9->UseVisualStyleBackColor = true;
			this->checkBox9->Visible = false;
			this->checkBox9->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox9_CheckedChanged);
			// 
			// checkBox8
			// 
			this->checkBox8->AutoSize = true;
			this->checkBox8->Checked = true;
			this->checkBox8->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox8->Location = System::Drawing::Point(3, 32);
			this->checkBox8->Name = L"checkBox8";
			this->checkBox8->Size = System::Drawing::Size(80, 17);
			this->checkBox8->TabIndex = 7;
			this->checkBox8->Text = L"checkBox8";
			this->checkBox8->UseVisualStyleBackColor = true;
			this->checkBox8->Visible = false;
			this->checkBox8->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox8_CheckedChanged);
			// 
			// checkBox7
			// 
			this->checkBox7->AutoSize = true;
			this->checkBox7->Checked = true;
			this->checkBox7->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox7->Location = System::Drawing::Point(3, 58);
			this->checkBox7->Name = L"checkBox7";
			this->checkBox7->Size = System::Drawing::Size(80, 17);
			this->checkBox7->TabIndex = 6;
			this->checkBox7->Text = L"checkBox7";
			this->checkBox7->UseVisualStyleBackColor = true;
			this->checkBox7->Visible = false;
			this->checkBox7->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox7_CheckedChanged);
			// 
			// checkBox6
			// 
			this->checkBox6->AutoSize = true;
			this->checkBox6->Checked = true;
			this->checkBox6->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox6->Location = System::Drawing::Point(3, 85);
			this->checkBox6->Name = L"checkBox6";
			this->checkBox6->Size = System::Drawing::Size(80, 17);
			this->checkBox6->TabIndex = 5;
			this->checkBox6->Text = L"checkBox6";
			this->checkBox6->UseVisualStyleBackColor = true;
			this->checkBox6->Visible = false;
			this->checkBox6->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox6_CheckedChanged);
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Checked = true;
			this->checkBox5->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox5->Location = System::Drawing::Point(3, 111);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(80, 17);
			this->checkBox5->TabIndex = 4;
			this->checkBox5->Text = L"checkBox5";
			this->checkBox5->UseVisualStyleBackColor = true;
			this->checkBox5->Visible = false;
			this->checkBox5->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox5_CheckedChanged);
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Checked = true;
			this->checkBox4->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox4->Location = System::Drawing::Point(3, 137);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(80, 17);
			this->checkBox4->TabIndex = 3;
			this->checkBox4->Text = L"checkBox4";
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->Visible = false;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox4_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Checked = true;
			this->checkBox3->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox3->Location = System::Drawing::Point(3, 163);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(80, 17);
			this->checkBox3->TabIndex = 2;
			this->checkBox3->Text = L"checkBox3";
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->Visible = false;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox3_CheckedChanged);
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Checked = true;
			this->checkBox2->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox2->Location = System::Drawing::Point(3, 189);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(80, 17);
			this->checkBox2->TabIndex = 1;
			this->checkBox2->Text = L"checkBox2";
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->Visible = false;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox2_CheckedChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Checked = true;
			this->checkBox1->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox1->Location = System::Drawing::Point(3, 215);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(80, 17);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"checkBox1";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->Visible = false;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox1_CheckedChanged);
			// 
			// panel_Step
			// 
			this->panel_Step->Controls->Add(this->textBox9);
			this->panel_Step->Controls->Add(this->textBox8);
			this->panel_Step->Controls->Add(this->textBox7);
			this->panel_Step->Controls->Add(this->textBox6);
			this->panel_Step->Controls->Add(this->textBox5);
			this->panel_Step->Controls->Add(this->textBox4);
			this->panel_Step->Controls->Add(this->textBox3);
			this->panel_Step->Controls->Add(this->textBox2);
			this->panel_Step->Controls->Add(this->textBox1);
			this->panel_Step->Location = System::Drawing::Point(119, 4);
			this->panel_Step->Name = L"panel_Step";
			this->panel_Step->Size = System::Drawing::Size(34, 237);
			this->panel_Step->TabIndex = 2;
			this->panel_Step->Visible = false;
			// 
			// textBox9
			// 
			this->textBox9->Location = System::Drawing::Point(3, 3);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(28, 20);
			this->textBox9->TabIndex = 8;
			this->textBox9->Text = L"10";
			this->textBox9->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox9->Visible = false;
			// 
			// textBox8
			// 
			this->textBox8->Location = System::Drawing::Point(3, 29);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(28, 20);
			this->textBox8->TabIndex = 7;
			this->textBox8->Text = L"10";
			this->textBox8->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox8->Visible = false;
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(3, 55);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(28, 20);
			this->textBox7->TabIndex = 6;
			this->textBox7->Text = L"10";
			this->textBox7->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox7->Visible = false;
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(3, 82);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(28, 20);
			this->textBox6->TabIndex = 5;
			this->textBox6->Text = L"10";
			this->textBox6->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox6->Visible = false;
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(3, 108);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(28, 20);
			this->textBox5->TabIndex = 4;
			this->textBox5->Text = L"10";
			this->textBox5->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox5->Visible = false;
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(3, 134);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(28, 20);
			this->textBox4->TabIndex = 3;
			this->textBox4->Text = L"10";
			this->textBox4->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox4->Visible = false;
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(3, 160);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(28, 20);
			this->textBox3->TabIndex = 2;
			this->textBox3->Text = L"10";
			this->textBox3->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox3->Visible = false;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(3, 186);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(28, 20);
			this->textBox2->TabIndex = 1;
			this->textBox2->Text = L"10";
			this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox2->Visible = false;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(3, 212);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(28, 20);
			this->textBox1->TabIndex = 0;
			this->textBox1->Text = L"10";
			this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox1->Visible = false;
			// 
			// panel_Vol
			// 
			this->panel_Vol->Controls->Add(this->checkBox18);
			this->panel_Vol->Controls->Add(this->checkBox17);
			this->panel_Vol->Controls->Add(this->checkBox16);
			this->panel_Vol->Controls->Add(this->checkBox15);
			this->panel_Vol->Controls->Add(this->checkBox14);
			this->panel_Vol->Controls->Add(this->checkBox13);
			this->panel_Vol->Controls->Add(this->checkBox12);
			this->panel_Vol->Controls->Add(this->checkBox11);
			this->panel_Vol->Controls->Add(this->checkBox10);
			this->panel_Vol->Location = System::Drawing::Point(93, 4);
			this->panel_Vol->Name = L"panel_Vol";
			this->panel_Vol->Size = System::Drawing::Size(19, 237);
			this->panel_Vol->TabIndex = 0;
			this->panel_Vol->Visible = false;
			this->panel_Vol->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ReSounder::panel1_Paint);
			// 
			// checkBox18
			// 
			this->checkBox18->AutoSize = true;
			this->checkBox18->Location = System::Drawing::Point(3, 6);
			this->checkBox18->Name = L"checkBox18";
			this->checkBox18->Size = System::Drawing::Size(15, 14);
			this->checkBox18->TabIndex = 8;
			this->checkBox18->UseVisualStyleBackColor = true;
			this->checkBox18->Visible = false;
			this->checkBox18->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox18_CheckedChanged);
			// 
			// checkBox17
			// 
			this->checkBox17->AutoSize = true;
			this->checkBox17->Location = System::Drawing::Point(3, 32);
			this->checkBox17->Name = L"checkBox17";
			this->checkBox17->Size = System::Drawing::Size(15, 14);
			this->checkBox17->TabIndex = 7;
			this->checkBox17->UseVisualStyleBackColor = true;
			this->checkBox17->Visible = false;
			this->checkBox17->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox17_CheckedChanged);
			// 
			// checkBox16
			// 
			this->checkBox16->AutoSize = true;
			this->checkBox16->Location = System::Drawing::Point(3, 58);
			this->checkBox16->Name = L"checkBox16";
			this->checkBox16->Size = System::Drawing::Size(15, 14);
			this->checkBox16->TabIndex = 6;
			this->checkBox16->UseVisualStyleBackColor = true;
			this->checkBox16->Visible = false;
			this->checkBox16->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox16_CheckedChanged);
			// 
			// checkBox15
			// 
			this->checkBox15->AutoSize = true;
			this->checkBox15->Location = System::Drawing::Point(3, 85);
			this->checkBox15->Name = L"checkBox15";
			this->checkBox15->Size = System::Drawing::Size(15, 14);
			this->checkBox15->TabIndex = 5;
			this->checkBox15->UseVisualStyleBackColor = true;
			this->checkBox15->Visible = false;
			this->checkBox15->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox15_CheckedChanged);
			// 
			// checkBox14
			// 
			this->checkBox14->AutoSize = true;
			this->checkBox14->Location = System::Drawing::Point(3, 111);
			this->checkBox14->Name = L"checkBox14";
			this->checkBox14->Size = System::Drawing::Size(15, 14);
			this->checkBox14->TabIndex = 4;
			this->checkBox14->UseVisualStyleBackColor = true;
			this->checkBox14->Visible = false;
			this->checkBox14->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox14_CheckedChanged);
			// 
			// checkBox13
			// 
			this->checkBox13->AutoSize = true;
			this->checkBox13->Location = System::Drawing::Point(3, 137);
			this->checkBox13->Name = L"checkBox13";
			this->checkBox13->Size = System::Drawing::Size(15, 14);
			this->checkBox13->TabIndex = 3;
			this->checkBox13->UseVisualStyleBackColor = true;
			this->checkBox13->Visible = false;
			this->checkBox13->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox13_CheckedChanged);
			// 
			// checkBox12
			// 
			this->checkBox12->AutoSize = true;
			this->checkBox12->Location = System::Drawing::Point(3, 163);
			this->checkBox12->Name = L"checkBox12";
			this->checkBox12->Size = System::Drawing::Size(15, 14);
			this->checkBox12->TabIndex = 2;
			this->checkBox12->UseVisualStyleBackColor = true;
			this->checkBox12->Visible = false;
			this->checkBox12->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox12_CheckedChanged);
			// 
			// checkBox11
			// 
			this->checkBox11->AutoSize = true;
			this->checkBox11->Location = System::Drawing::Point(3, 189);
			this->checkBox11->Name = L"checkBox11";
			this->checkBox11->Size = System::Drawing::Size(15, 14);
			this->checkBox11->TabIndex = 1;
			this->checkBox11->UseVisualStyleBackColor = true;
			this->checkBox11->Visible = false;
			this->checkBox11->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox11_CheckedChanged);
			// 
			// checkBox10
			// 
			this->checkBox10->AutoSize = true;
			this->checkBox10->Location = System::Drawing::Point(3, 215);
			this->checkBox10->Name = L"checkBox10";
			this->checkBox10->Size = System::Drawing::Size(15, 14);
			this->checkBox10->TabIndex = 0;
			this->checkBox10->UseVisualStyleBackColor = true;
			this->checkBox10->Visible = false;
			this->checkBox10->CheckedChanged += gcnew System::EventHandler(this, &ReSounder::checkBox10_CheckedChanged);
			// 
			// b_Process
			// 
			this->b_Process->Location = System::Drawing::Point(198, 127);
			this->b_Process->Name = L"b_Process";
			this->b_Process->Size = System::Drawing::Size(130, 48);
			this->b_Process->TabIndex = 12;
			this->b_Process->Text = L"PROCESS";
			this->b_Process->UseVisualStyleBackColor = true;
			this->b_Process->Click += gcnew System::EventHandler(this, &ReSounder::b_Process_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(198, 181);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(505, 38);
			this->progressBar1->TabIndex = 13;
			this->progressBar1->Text = L"asdasd";
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->Filter = L"fxp files (*.fxp)|*.fxp|All files (*.*)|*.*";
			// 
			// b_Save
			// 
			this->b_Save->Location = System::Drawing::Point(334, 127);
			this->b_Save->Name = L"b_Save";
			this->b_Save->Size = System::Drawing::Size(130, 48);
			this->b_Save->TabIndex = 14;
			this->b_Save->Text = L"SAVE";
			this->b_Save->UseVisualStyleBackColor = true;
			this->b_Save->Visible = false;
			this->b_Save->Click += gcnew System::EventHandler(this, &ReSounder::b_Save_Click);
			// 
			// check_Fast
			// 
			this->check_Fast->AutoSize = true;
			this->check_Fast->Checked = true;
			this->check_Fast->CheckState = System::Windows::Forms::CheckState::Checked;
			this->check_Fast->Location = System::Drawing::Point(139, 102);
			this->check_Fast->Name = L"check_Fast";
			this->check_Fast->Size = System::Drawing::Size(84, 17);
			this->check_Fast->TabIndex = 15;
			this->check_Fast->Text = L"Fast method";
			this->check_Fast->UseVisualStyleBackColor = true;
			this->check_Fast->Visible = false;
			// 
			// ReSounder
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(719, 433);
			this->Controls->Add(this->check_Fast);
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
			this->Name = L"ReSounder";
			this->Text = L"ReSounder";
			this->group_Params->ResumeLayout(false);
			this->group_Params->PerformLayout();
			this->table_Params->ResumeLayout(false);
			this->panel_Name->ResumeLayout(false);
			this->panel_Name->PerformLayout();
			this->panel_Step->ResumeLayout(false);
			this->panel_Step->PerformLayout();
			this->panel_Vol->ResumeLayout(false);
			this->panel_Vol->PerformLayout();
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
							for (int j=0; j<3; j++) {
								table_Params->Controls[j]->Controls[i]->Visible = false;
							}
						}
						//ukazeme niektore checkboxy
						for (int i=0; i<numParams; i++) {
							table_Params->Controls[0]->Controls[i]->Text = host->getParamName(i);
							for (int j=0; j<3; j++) {
								table_Params->Controls[j]->Controls[i]->Visible = true;
							}
						}
					}
				}
			 }
		 }

private: System::Void b_Process_Click(System::Object^  sender, System::EventArgs^  e) {
			 float sSize;
			 bool sSizeOk = true;

			 //skontrolujeme step sizes
			 if (panel_Step->Visible == true) {
				 for (int i=0; i<numParams; i++) {
					 sSize = Convert::ToSingle(panel_Step->Controls[i]->Text);
					 if ( (sSize<=0 || sSize>100) && ((System::Windows::Forms::CheckBox^)panel_Name->Controls[i])->Checked ){
						 //ak je step mimo rozsah a zaroven je dany parameter zvoleny
						 sSizeOk = false;
						 break;
					 }
				 }
			 }
			

			 if (host->getOk() && (combo_Type->SelectedIndex!=-1) && sSizeOk) {	//ak su zvolene vsetky parametre programu
				 int* selectedParams = new int[numParams];
				 int* stepSize = new int[numParams];
				 bool*	volType = new bool[numParams];
				 
				 //zistime selectedParams, numSelectedParams a stepSize
				 numSelectedParams = 0;
				 for (int i=0; i<numParams; i++) {
					 if (((System::Windows::Forms::CheckBox^)panel_Name->Controls[i])->Checked) {
						 stepSize[numSelectedParams] = (Convert::ToInt32(panel_Step->Controls[i]->Text));
						 volType[numSelectedParams] = ((System::Windows::Forms::CheckBox^)panel_Vol->Controls[i])->Checked;
						 selectedParams[numSelectedParams++] = i;
					 }
				 }

				 //spustime process
				 host->setProcessed(false);
				 if (numSelectedParams>0) {
					progressBar1->Value = 0;
					host->process(combo_Type->SelectedIndex, selectedParams, volType, stepSize, numSelectedParams, check_Fast->Checked, progressBar1);
				 
					console->AppendText(host->message + nl);
				 }
				 else {
					 console->AppendText("ERROR: no parameter selected" + nl);	
				 }
				 delete [] stepSize;
				 delete [] volType;
				 delete [] selectedParams;
				 if (host->getProcessed()) {
					 b_Save->Visible = true;
				 }
				 else {
					 b_Save->Visible = false;
				 }
			 }
			 else {
				 console->AppendText(gcnew String("ERROR: some of program's parameters are invalid!") + nl);
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
private: void group_Params_redraw() {
			 for (int i=0; i<numParams; i++) {
				 if (((System::Windows::Forms::CheckBox^)panel_Name->Controls[i])->Checked) {
					 panel_Vol->Controls[i]->Enabled = true;
					 panel_Step->Controls[i]->Enabled = true;
				 }
				 else {
					 panel_Vol->Controls[i]->Enabled = false;
					 panel_Step->Controls[i]->Enabled = false;
				 }
			 }
		 }

private: void paramsToProcess_reset() {
			 for (int i=0; i<maxParams; i++) {
				 ((System::Windows::Forms::CheckBox^)panel_Name->Controls[i])->Checked = true;
				 ((System::Windows::Forms::CheckBox^)panel_Name->Controls[i])->Text = "";
				 ((System::Windows::Forms::CheckBox^)panel_Vol->Controls[i])->Checked = false;
				 panel_Step->Controls[i]->Text = "10";
				 panel_Name->Controls[i]->Enabled = true;
			 }
		 }
private: System::Void checkBox10_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox11_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox12_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox13_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox14_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox15_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox16_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox17_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox18_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		 }
private: System::Void checkBox8_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox9_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox7_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox6_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 group_Params_redraw();
		 }
private: System::Void combo_Type_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 paramsToProcess_reset();
			 if (combo_Type->SelectedIndex == 0 || combo_Type->SelectedIndex==1) {
				 //skryjeme vsetky checkboxy
					for (int i=0; i<maxParams; i++) {
						for (int j=0; j<3; j++) {
							table_Params->Controls[j]->Controls[i]->Visible = false;
						}
					}
					
				 l_EffectFile->Text = "- select file -";				
				 b_Effect->Visible = true;
				 l_Effect->Visible = true;
				 l_EffectFile->Visible = true;
				 panel_Step->Visible = true;
				 panel_Vol->Visible = true;

				 
			 }
			 else {
				 b_Effect->Visible = false;
				 l_Effect->Visible = false;
				 l_EffectFile->Visible = false;
				 panel_Step->Visible = false;
				 panel_Vol->Visible = false;

				 String^ fileName;
				 switch (combo_Type->SelectedIndex) {
					 case 2 : fileName = gcnew String(homeDir + "\\doubledelay.dll"); break;
					 case 3 : fileName = gcnew String(homeDir + "\\equalizer.dll"); break;
					 case 4 : fileName = gcnew String(homeDir + "\\compressor.dll"); break;
					 case 5 : fileName = gcnew String(homeDir + "\\compressor.dll"); break;
					 case 6 : fileName = gcnew String(homeDir + "\\pitch_shifter.dll"); break;
				 }

				try {
					host->openEffectFile(fileName);
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
							for (int j=0; j<3; j++) {
								table_Params->Controls[j]->Controls[i]->Visible = false;
							}
						}
						//ukazeme niektore checkboxy, pre limiter sfunkcnime iba treshold
						for (int i=0; i<numParams; i++) {
							table_Params->Controls[0]->Controls[i]->Text = host->getParamName(i);	//meno
							for (int j=0; j<3; j++) {
								table_Params->Controls[j]->Controls[i]->Visible = true;
							}

							if (combo_Type->SelectedIndex == 4) {	//ak mame zvoleny limiter
								if (i!=5) {	//vsetko okrem treshold-u
									table_Params->Controls[0]->Controls[i]->Enabled = false;	//znefunkcnenie
									((System::Windows::Forms::CheckBox^)table_Params->Controls[0]->Controls[i])->Checked = false;	//odskrtnutie
								}
							}
							
						}

					}
				}

			 }

			 if (combo_Type->SelectedIndex==0 || combo_Type->SelectedIndex==1 || combo_Type->SelectedIndex==2 || combo_Type->SelectedIndex==6) {	//ak general, single, delay a pitch
				 check_Fast->Visible = true;
				 
				 //resetneme velkost kroku
				 //nastavime kroky
				 for (int i=0; i<8; i++) {
					 panel_Step->Controls[i]->Text = gcnew String("10");
				 }
				 if (combo_Type->SelectedIndex == 2) {	//ak zvoleny delayu
					textBox9->Text = gcnew String("1");		//krok parametru delay nastavime na 1
					checkBox17->Checked = true;				//volume type pre parameter wet
					checkBox16->Checked = true;				//volume type pre parameter dry
				 }
				 
			 }
			 else {
				 check_Fast->Visible = false;
			 }

		 }
private: System::Void l_Wet_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

