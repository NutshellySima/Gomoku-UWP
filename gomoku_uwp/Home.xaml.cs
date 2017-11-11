using System;
using System.Collections.Generic;
using Windows.UI;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using unmanaged;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace gomoku_uwp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Home : Page
    {
        class chessboard_line_class
        {
            public UIElement line;
            public int i;
            public bool horizontal;
            public chessboard_line_class(UIElement line, int i, bool horizontal)
            {
                this.line = line;
                this.i = i;
                this.horizontal = horizontal;
            }
        }
        class chessboard_point_class
        {
            public UIElement point;
            public int row, col;
            public chessboard_point_class(UIElement point, int row, int col)
            {
                this.point = point;
                this.row = row;
                this.col = col;
            }
        }
        class chessboard_notice_line_class
        {
            public UIElement notice_line;
            public int row, col;
            public bool first;
            public chessboard_notice_line_class(UIElement notice_line, int row, int col, bool first)
            {
                this.notice_line = notice_line;
                this.row = row;
                this.col = col;
                this.first = first;
            }
        }
        // Global definitions
        bool computerrunning = false;
        private int turn = 1;
        // Resource
        Windows.Storage.ApplicationDataContainer gameOptions = Windows.Storage.ApplicationData.Current.LocalSettings;
        private unmanaged.cliwrapper invoke = new unmanaged.cliwrapper();
        // UIElement List
        private List<chessboard_line_class> chessboard_line = new List<chessboard_line_class>();
        private List<chessboard_point_class> chessboard_point = new List<chessboard_point_class>();
        private List<chessboard_notice_line_class> chessboard_noticeline = new List<chessboard_notice_line_class>();
        public Home()
        {
            this.InitializeComponent();
            InitWindow();
        }
        // specific Draw related
        public void Drawchessboard(int i, bool horizontal)
        {
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            Windows.UI.Xaml.Shapes.Line line;
            line = new Windows.UI.Xaml.Shapes.Line();
            if (horizontal == true)
            {
                line.X1 = deltax;
                line.X2 = 15 * deltax;
                line.Y1 = deltay * i;
                line.Y2 = deltay * i;
            }
            else
            {
                line.Y1 = deltay;
                line.Y2 = 15 * deltay;
                line.X1 = deltax * i;
                line.X2 = deltax * i;
            }
            line.X1 += 10;
            line.X2 += 10;
            line.Y1 += 10;
            line.Y2 += 10;
            line.IsHitTestVisible = false;
            line.Stroke = new SolidColorBrush(Colors.Black);
            line.StrokeThickness = 1;
            chessboard_father.Children.Add(line);
            chessboard_line.Add(new chessboard_line_class(line, i, horizontal));
        }
        public void DrawPoint(int row, int col, bool black)
        {
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            Windows.UI.Xaml.Shapes.Ellipse circle;
            circle = new Windows.UI.Xaml.Shapes.Ellipse()
            {
                Height = 0.75 * deltay,
                Width = 0.75 * deltax
            };
            double left = (row - 7) * 2 * deltax;
            double top = (col - 7) * 2 * deltay;
            circle.Margin = new Thickness(left, top, 0, 0);
            if (black == true)
                circle.Fill = new SolidColorBrush(Colors.Black);
            else
                circle.Fill = new SolidColorBrush(Colors.White);
            chessboard_father.Children.Add(circle);
            chessboard_point.Add(new chessboard_point_class(circle, row, col));
        }
        public void DrawNoticeLine(int row, int col, bool black)
        {
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            Windows.UI.Xaml.Shapes.Line line;
            line = new Windows.UI.Xaml.Shapes.Line()
            {
                X1 = 10 + (row + 0.875) * deltax,
                X2 = 10 + (row + 0.875) * deltax + 0.25 * deltax,
                Y1 = 10 + (col + 1) * deltay,
                Y2 = 10 + (col + 1) * deltay
            };
            if (black)
                line.Stroke = new SolidColorBrush(Colors.White);
            else
                line.Stroke = new SolidColorBrush(Colors.Black);
            line.StrokeThickness = 1.2;
            chessboard_father.Children.Add(line);
            chessboard_noticeline.Add(new chessboard_notice_line_class(line, row, col, true));
            line = new Windows.UI.Xaml.Shapes.Line()
            {
                Y1 = 10 + (col + 0.875) * deltay,
                Y2 = 10 + (col + 0.875) * deltay + 0.25 * deltay,
                X1 = 10 + (row + 1) * deltax,
                X2 = 10 + (row + 1) * deltax
            };
            if (black)
                line.Stroke = new SolidColorBrush(Colors.White);
            else
                line.Stroke = new SolidColorBrush(Colors.Black);
            line.StrokeThickness = 1.2;
            chessboard_father.Children.Add(line);
            chessboard_noticeline.Add(new chessboard_notice_line_class(line, row, col, false));
        }
        // specific Remove related
        public void Remove_noticeLine(int i)
        {
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_noticeline[chessboard_noticeline.Count - 1].notice_line);
                chessboard_noticeline.RemoveAt(chessboard_noticeline.Count - 1);
            }
        }
        public void Remove_chessboardLine(int i)
        {
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_line[chessboard_line.Count - 1].line);
                chessboard_line.RemoveAt(chessboard_line.Count - 1);
            }
        }
        public void Remove_chessboardPoint(int i)
        {
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_point[chessboard_point.Count - 1].point);
                chessboard_point.RemoveAt(chessboard_point.Count - 1);
            }
        }
        // specific Clear related
        public void Clear_noticeLine()
        {
            var i = chessboard_noticeline.Count;
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_noticeline[chessboard_noticeline.Count - 1].notice_line);
                chessboard_noticeline.RemoveAt(chessboard_noticeline.Count - 1);
            }
        }
        // Main logic related
        public void InitWindow()
        {
            for (int ii = 1; ii <= 15; ++ii)
            {
                Drawchessboard(ii, true);
                Drawchessboard(ii, false);
            }
        }
        public void WinPrinter()
        {
            Clear_noticeLine();
            invoke.Checkwin(true);
            var board = invoke.Getchessboard();
            for (int ii = 0; ii < 15; ++ii)
            {
                for (int iii = 0; iii < 15; ++iii)
                {
                    if (board[15 * ii + iii] == 3)
                        DrawNoticeLine(iii, ii, true);
                    if (board[15 * ii + iii] == 4)
                        DrawNoticeLine(iii, ii, false);
                }
            }
            invoke.Checkwin(false);
        }
    }
}
