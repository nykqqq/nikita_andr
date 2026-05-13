<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>Program1MainWindow</class>
 <widget class="QMainWindow" name="Program1MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>500</width>
    <height>400</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Программа 1 - Создание объектов Person</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <widget class="QGroupBox" name="groupBox">
      <property name="title">
       <string>Данные объекта</string>
      </property>
      <layout class="QGridLayout" name="gridLayout">
       <item row="0" column="0">
        <widget class="QLabel" name="labelName">
         <property name="text">
          <string>Имя:</string>
         </property>
         <property name="font">
          <font>
           <pointsize>10</pointsize>
           <bold>true</bold>
          </font>
         </property>
        </widget>
       </item>
       <item row="0" column="1">
        <widget class="QLineEdit" name="nameEdit">
         <property name="placeholderText">
          <string>Введите имя</string>
         </property>
        </widget>
       </item>
       <item row="1" column="0">
        <widget class="QLabel" name="labelSurname">
         <property name="text">
          <string>Фамилия:</string>
         </property>
         <property name="font">
          <font>
           <pointsize>10</pointsize>
           <bold>true</bold>
          </font>
         </property>
        </widget>
       </item>
       <item row="1" column="1">
        <widget class="QLineEdit" name="surnameEdit">
         <property name="placeholderText">
          <string>Введите фамилию</string>
         </property>
        </widget>
       </item>
       <item row="2" column="0">
        <widget class="QLabel" name="labelHeight">
         <property name="text">
          <string>Рост (см):</string>
         </property>
         <property name="font">
          <font>
           <pointsize>10</pointsize>
           <bold>true</bold>
          </font>
         </property>
        </widget>
       </item>
       <item row="2" column="1">
        <widget class="QSpinBox" name="heightSpin">
         <property name="minimum">
          <number>0</number>
         </property>
         <property name="maximum">
          <number>300</number>
         </property>
         <property name="suffix">
          <string> см</string>
         </property>
        </widget>
       </item>
       <item row="3" column="0">
        <widget class="QLabel" name="labelWeight">
         <property name="text">
          <string>Вес (кг):</string>
         </property>
         <property name="font">
          <font>
           <pointsize>10</pointsize>
           <bold>true</bold>
          </font>
         </property>
        </widget>
       </item>
       <item row="3" column="1">
        <widget class="QDoubleSpinBox" name="weightSpin">
         <property name="minimum">
          <double>0.000000000000000</double>
         </property>
         <property name="maximum">
          <double>500.000000000000000</double>
         </property>
         <property name="suffix">
          <string> кг</string>
         </property>
         <property name="decimals">
          <number>1</number>
         </property>
        </widget>
       </item>
      </layout>
     </widget>
    </item>
    <item>
     <widget class="QGroupBox" name="groupBox_2">
      <property name="title">
       <string>Управление JSON</string>
      </property>
      <layout class="QHBoxLayout" name="horizontalLayout">
       <item>
        <widget class="QPushButton" name="loadJsonBtn">
         <property name="text">
          <string>Загрузить JSON</string>
         </property>
         <property name="icon">
          <iconset theme="document-open"/>
         </property>
        </widget>
       </item>
       <item>
        <widget class="QPushButton" name="saveJsonBtn">
         <property name="text">
          <string>Сохранить JSON</string>
         </property>
         <property name="icon">
          <iconset theme="document-save"/>
         </property>
        </widget>
       </item>
      </layout>
     </widget>
    </item>
    <item>
     <widget class="QGroupBox" name="groupBox_3">
      <property name="title">
       <string>Импорт из TXT</string>
      </property>
      <layout class="QVBoxLayout" name="verticalLayout_2">
       <item>
        <widget class="QPushButton" name="loadTxtBtn">
         <property name="text">
          <string>Загрузить из TXT файла</string>
         </property>
         <property name="icon">
          <iconset theme="document-open"/>
         </property>
        </widget>
       </item>
       <item>
        <widget class="QLabel" name="labelTxtFormat">
         <property name="text">
          <string>Формат TXT: Имя/Фамилия/Рост/Вес (например: Вася/Петров/174/43.0)</string>
         </property>
         <property name="wordWrap">
          <bool>true</bool>
         </property>
         <property name="styleSheet">
          <string notr="true">color: gray; font-size: 9px;</string>
         </property>
        </widget>
       </item>
      </layout>
     </widget>
    </item>
    <item>
     <widget class="QPushButton" name="addPersonBtn">
      <property name="text">
       <string>Добавить объект в JSON</string>
      </property>
      <property name="font">
       <font>
        <pointsize>11</pointsize>
        <bold>true</bold>
       </font>
      </property>
      <property name="styleSheet">
       <string notr="true">background-color: #4CAF50; color: white; padding: 8px;</string>
      </property>
     </widget>
    </item>
   </layout>
  </widget>
  <widget class="QMenuBar" name="menubar">
   <property name="geometry">
    <rect>
     <x>0</x>
     <y>0</y>
     <width>500</width>
     <height>21</height>
    </rect>
   </property>
   <widget class="QMenu" name="menuFile">
    <property name="title">
     <string>Файл</string>
    </property>
    <addaction name="actionExit"/>
   </widget>
   <widget class="QMenu" name="menuHelp">
    <property name="title">
     <string>Помощь</string>
    </property>
    <addaction name="actionAbout"/>
   </widget>
   <addaction name="menuFile"/>
   <addaction name="menuHelp"/>
  </widget>
  <widget class="QStatusBar" name="statusbar"/>
  <action name="actionExit">
   <property name="text">
    <string>Выход</string>
   </property>
  </action>
  <action name="actionAbout">
   <property name="text">
    <string>О программе</string>
   </property>
  </action>
 </widget>
 <resources/>
 <connections/>
</ui>
