VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form Form1 
   Caption         =   "o2m8 WebBrick"
   ClientHeight    =   10710
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10515
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   ScaleHeight     =   10710
   ScaleWidth      =   10515
   StartUpPosition =   3  'Windows Default
   Begin MSWinsockLib.Winsock UdpWinsock 
      Left            =   7800
      Top             =   360
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
   End
   Begin VB.TextBox LogText 
      CausesValidation=   0   'False
      Height          =   8415
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   6
      Text            =   "HPI_WebBrick_NC.frx":0000
      Top             =   2160
      Width           =   10335
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   2160
      TabIndex        =   5
      Text            =   "193.123.195.209"
      Top             =   840
      Width           =   1455
   End
   Begin VB.CommandButton btnTestCom 
      Caption         =   "&Test"
      Height          =   315
      Left            =   3720
      TabIndex        =   3
      Top             =   840
      Width           =   735
   End
   Begin VB.CommandButton btnClose 
      Caption         =   "&Close"
      Height          =   375
      Left            =   8880
      TabIndex        =   1
      Top             =   360
      Width           =   1455
   End
   Begin VB.Label Label3 
      Caption         =   "Data Received"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   1800
      Width           =   1455
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Right Justify
      Caption         =   "IP Address"
      Height          =   255
      Left            =   480
      TabIndex        =   2
      Top             =   840
      Width           =   1455
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "o2m8 WebBrick"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   360
      TabIndex        =   0
      Top             =   240
      Width           =   3975
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ===========================================================
' ===
' === Harmony Plugin Example
' === Version 1.0 - D.K. Mattocks
' === 22 April 2005, iDomus Limited
' === For support visit www.iDomus.co.uk
' ===
' === This is the form for the Plugin example.  It can be one of
' === many forms, or indeed could be deleted altogether should
' === you not require any user interface.
' ===
' === You may extend the functionality of the form as much as you
' === wish using all the traditional VB form controls.
' ===
' ===========================================================

Option Explicit

' ===========================================================
' ===
' === Not much clever code here.
' === Just remember not to terminate the application here, just hide
' === it is required
' ===
' ===========================================================

Private Sub btnClose_Click()
Form1.Visible = False
End Sub

Private Sub UdpWinsock_DataArrival(ByVal bytesTotal As Long)
    Dim strData As String
    Dim uType As String
    Dim pType As String
    Dim srcChn As Integer
    Dim tgtChn As Integer
    Dim fromNode As Integer
    Dim toNode As Integer
    Dim action As Integer
    Dim dwell As Integer
    Dim sp As Integer
    Dim val As Integer
    
    UdpWinsock.GetData strData
    uType = Mid(strData, 2, 1)
    pType = Mid(strData, 3, 2)
    srcChn = Asc(Mid(strData, 5, 1))
    tgtChn = Asc(Mid(strData, 6, 1))
    fromNode = Asc(Mid(strData, 8, 1))
    toNode = Asc(Mid(strData, 9, 1))
    action = Asc(Mid(strData, 7, 1))
    dwell = action / 16
    action = action And 15
    sp = Asc(Mid(strData, 10, 1))
    val = 256 * Asc(Mid(strData, 11, 1)) + Asc(Mid(strData, 12, 1))
    
'    ToLog uType & " " & pType & " s:" & srcChn & " t:" & tgtChn & " f:" & fromNode & " " & toNode & " a:" & action & " d:" & dwell & " " & sp & " v:" & val
    
    Select Case pType
        Case "Ta":
        Case "TA":
            ToLog uType & " s:" & srcChn & " f:" & fromNode & " v:" & val
        Case "Td":
        Case "TD":
            ToLog uType & " s:" & srcChn & " f:" & fromNode & " a:" & action
            With frmHarmony
                .Raise_Event WEBBRICK_CTRL, "DigitalInputs", "DigTrigger " & (srcChn + 1)
            End With
        Case "Tt":
        Case "TT":
        Case "TS":
        Case "AI":
        Case "AO":
            ToLog uType & " s:" & srcChn & " f:" & fromNode & " v:" & val
        Case "DO":
        Case "NN":
        Case "SS":
        Case "NS":
        Case "MM":
    End Select
    
'    unsigned char len;          // placeholder so I can try some webrick to webrick transfers
'    unsigned char udpType;      // G, A, R
'    unsigned char source[2];    // Ta, low Trigger from analogue
'                                // TA, high Trigger from analogue
'                                // Td, Trigger from remote DI command
'                                // TD, Trigger from local digital input
'                                // Tt  low Trigger from temperature
'                                // TT  high Trigger from temperature
'                                // TS  Trigger from scheduled event
'                                // AI, new input value
'                                // AO, new output value
'                                // DO, new digital output state
'                                // NN, unconfigured node.
'                                // SS, node starting.
'                                // MM, monitor state change, will become obsolete
'    unsigned char srcChannel;   // identifies source channel includes setpoint Nr.
'    unsigned char tgtChannel;   // identifies source channel includes setpoint Nr.
'    unsigned char action;       // to be performed includes Dwell value
'    unsigned char fromNodeNr;   // always self
'    unsigned char toNodeNr;     // non zero if R command
'    unsigned char setPointNr;   // only
'    unsigned char curValH;      // Can be temperature or analog value
'    unsigned char curValL;      // Low part of value
    
    
End Sub

Public Sub ToLog(txt As String)

    LogText.Text = txt & vbCrLf & LogText.Text
    If Len(LogText.Text) > 30000 Then LogText.Text = Left(LogText.Text, 20000)

End Sub

' ===========================================================
' === End
' ===========================================================

Private Sub Form_Load()
    On Error Resume Next
    With UdpWinsock
        .RemoteHost = "Local"
        .RemotePort = 26482
        .Bind 2552
        ToLog "UdpWinSock bind " & Err.Number
    End With
End Sub
