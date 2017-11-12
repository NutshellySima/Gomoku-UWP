using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.UI;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Input;
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
        IReadOnlyList<string> runtimeLanguages = Windows.ApplicationModel.Resources.Core.ResourceContext.GetForCurrentView().Languages;
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
        bool operationCanceled = false;
        bool isChinese = false;
        private int turn = 1;
        // Resource
        Windows.Storage.ApplicationDataContainer _gameOptions = Windows.Storage.ApplicationData.Current.LocalSettings;
        private unmanaged.cliwrapper invoke = new unmanaged.cliwrapper();
        // UIElement List
        private List<chessboard_line_class> chessboard_line = new List<chessboard_line_class>();
        private List<chessboard_point_class> chessboard_point = new List<chessboard_point_class>();
        private List<chessboard_notice_line_class> chessboard_noticeline = new List<chessboard_notice_line_class>();
        public Home()
        {
            this.InitializeComponent();
            runtimeLanguages = Windows.Globalization.ApplicationLanguages.Languages;
            if (runtimeLanguages[0].Length >= 2 && runtimeLanguages[0][0] == 'z' && runtimeLanguages[0][1] == 'h')
                isChinese = true;
            initSettings();
            InitWindow();
        }
        public void initSettings()
        {
            Object valueMode = _gameOptions.Values["mode"];
            Object valueBlack = _gameOptions.Values["black"];
            Object valueWhite = _gameOptions.Values["white"];
            if (valueMode == null || valueBlack == null || valueWhite == null)
            {
                _gameOptions.Values["mode"] = "hard";
                _gameOptions.Values["black"] = "computer";
                _gameOptions.Values["white"] = "human";
            }
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
            line.StrokeThickness = 2;
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
            if (!isChinese)
            {
                BlackLabel.Text = "Black: " + _gameOptions.Values["black"].ToString();
                WhiteLabel.Text = "White: " + _gameOptions.Values["white"].ToString();
                ModeLabel.Text = "Mode: " + _gameOptions.Values["mode"].ToString();
                EvalLabel.Text = "Eval: ---";
            }
            else
            {
                String ModeLab = _gameOptions.Values["mode"].ToString();
                String BlackLab = _gameOptions.Values["black"].ToString();
                String WhiteLab = _gameOptions.Values["white"].ToString();
                BlackLabel.Text = "黑棋：";
                WhiteLabel.Text = "白棋：";
                ModeLabel.Text = "模式：";
                if (BlackLab == "computer")
                    BlackLabel.Text += "人工智能";
                else
                    BlackLabel.Text += "人类玩家";
                if (WhiteLab == "computer")
                    WhiteLabel.Text += "人工智能";
                else
                    WhiteLabel.Text += "人类玩家";
                if (ModeLab == "easy")
                    ModeLabel.Text += "简单";
                else
                    ModeLabel.Text += "困难";
                EvalLabel.Text = "评分：---";
            }
            PlayGame();
        }
        public void WinPrinter()
        {
            Clear_noticeLine();
            UndoButton.IsEnabled = false;
            RestartButton.IsEnabled = false;
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
            UndoButton.IsEnabled = true;
            RestartButton.IsEnabled = true;
        }
        private async Task PlaybyComputer(bool black)
        {
            UndoButton.IsEnabled = false;
            RestartButton.IsEnabled = false;
            computerrunning = true;
            if (!isChinese)
                EvalLabel.Text = "Eval: Calculating.";
            else
                EvalLabel.Text = "评分：计算中";
            Task<int[]> com;
            if (black)
            {
                if (_gameOptions.Values["mode"].ToString() == "hard")
                    com = Task.Run(() => invoke.Putcomputer(true, 1));
                else
                    com = Task.Run(() => invoke.Putcomputer(false, 1));
            }
            else
            {
                if (_gameOptions.Values["mode"].ToString() == "hard")
                    com = Task.Run(() => invoke.Putcomputer(true, 2));
                else
                    com = Task.Run(() => invoke.Putcomputer(false, 2));
            }
            await com;
            var data = com.Result;
            //System.Text.Encoding encode = System.Text.Encoding.ASCII;
            //byte[] bytedata = encode.GetBytes(Convert.ToString(data.Item1));
            //EvalLabel.Content = "Eval: " + Convert.ToBase64String(bytedata, 0, bytedata.Length);
            if (!isChinese)
                EvalLabel.Text = "Eval: " + Convert.ToString(data[0]);
            else
                EvalLabel.Text = "评分：" + Convert.ToString(data[0]);
            Clear_noticeLine();
            if (black)
            {
                DrawPoint(data[2], data[1], true);
                DrawNoticeLine(data[2], data[1], true);
            }
            else
            {
                DrawPoint(data[2], data[1], false);
                DrawNoticeLine(data[2], data[1], false);
            }
            ++turn;
            UndoButton.IsEnabled = true;
            RestartButton.IsEnabled = true;
            computerrunning = false;
            if (black && invoke.Checkwin(false) == 1)
            {
                WinPrinter();
                if (!isChinese)
                {
                    ContentDialog Result = new ContentDialog
                    {
                        Title = "Result",
                        Content = "ヾ(*ΦωΦ)ツ\nBlack: Computer wins.",
                        CloseButtonText = "Ok"
                    };
                    ContentDialogResult result = await Result.ShowAsync();
                }
                else
                {
                    ContentDialog Result = new ContentDialog
                    {
                        Title = "结果",
                        Content = "ヾ(*ΦωΦ)ツ\n黑棋：人工智能胜利。",
                        CloseButtonText = "Ok"
                    };
                    ContentDialogResult result = await Result.ShowAsync();
                }
            }
            else if ((!black) && invoke.Checkwin(false) == 2)
            {
                WinPrinter();
                if (!isChinese)
                {
                    ContentDialog Result = new ContentDialog
                    {
                        Title = "Result",
                        Content = "ヾ(*ΦωΦ)ツ\nWhite: Computer wins.",
                        CloseButtonText = "Ok"
                    };
                    ContentDialogResult result = await Result.ShowAsync();
                }
                else
                {
                    ContentDialog Result = new ContentDialog
                    {
                        Title = "结果",
                        Content = "ヾ(*ΦωΦ)ツ\n白棋：人工智能胜利。",
                        CloseButtonText = "Ok"
                    };
                    ContentDialogResult result = await Result.ShowAsync();
                }
            }
            else
                PlayGame();
        }
        public async void PlayGame()
        {
            if (operationCanceled)
                return;
            if (invoke.Fullboard())
            {
                if (!isChinese)
                {
                    ContentDialog Fullboard = new ContentDialog
                    {
                        Title = "Fullboard",
                        Content = "ヾ(*ΦωΦ)ツ\nChess board is full.",
                        CloseButtonText = "Ok"
                    };
                    ContentDialogResult result = await Fullboard.ShowAsync();
                }
                else
                {
                    ContentDialog Fullboard = new ContentDialog
                    {
                        Title = "棋盘已满",
                        Content = "ヾ(*ΦωΦ)ツ\n棋盘已满。",
                        CloseButtonText = "Ok"
                    };
                    ContentDialogResult result = await Fullboard.ShowAsync();
                }
                return;
            }
            if (invoke.Checkwin(false) == 0)
            {
                //Black
                if (turn % 2 == 1)
                {
                    if (_gameOptions.Values["black"].ToString() == "computer")
                    {
#pragma warning disable CS4014 
                        PlaybyComputer(true);
#pragma warning restore CS4014 
                    }
                }
                //White
                else
                {
                    if (_gameOptions.Values["white"].ToString() == "computer")
                    {
#pragma warning disable CS4014 
                        PlaybyComputer(false);
#pragma warning restore CS4014 
                    }
                }
            }
            else
                return;
        }
        public bool UndoGame()
        {
            bool Undostatus = invoke.BoardUndo();
            if (Undostatus == false)
            {
                return false;
            }
            else
            {
                Clear_noticeLine();
                Remove_chessboardPoint(1);
                turn -= 1;
                if (invoke.Gethistory() != null && invoke.Gethistory().Length >= 3)
                {
                    var x3 = invoke.Gethistory()[invoke.Gethistory().Length - 1];
                    var x2 = invoke.Gethistory()[invoke.Gethistory().Length - 2];
                    var x1 = invoke.Gethistory()[invoke.Gethistory().Length - 3];
                    if (x1 == 1)
                        DrawNoticeLine(x3, x2, true);
                    else
                        DrawNoticeLine(x3, x2, false);
                }
                if(!isChinese)
                EvalLabel.Text = "Eval: ---";
                else
                    EvalLabel.Text = "评分：---";
                return true;
            }
        }
        private async void chessboard_background_PointerReleased(object sender, PointerRoutedEventArgs e)
        {
            if (computerrunning || invoke.Checkwin(false) != 0)
                return;
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            PointerPoint ptr = e.GetCurrentPoint(chessboard_background);
            Point pt = ptr.Position;
            if (turn % 2 == 1)
            {
                bool x = invoke.Puthuman((int)Math.Round(pt.Y / deltay) - 1, (int)Math.Round(pt.X / deltax) - 1, 1);
                if (x == true)
                {
                    Clear_noticeLine();
                    DrawPoint((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, true);
                    DrawNoticeLine((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, true);
                    ++turn;
                    if (invoke.Checkwin(false) == 1)
                    {
                        WinPrinter();
                        if (!isChinese)
                        {
                            ContentDialog Result = new ContentDialog
                            {
                                Title = "Result",
                                Content = "ヾ(*ΦωΦ)ツ\nBlack: Human wins.",
                                CloseButtonText = "Ok"
                            };
                            ContentDialogResult result = await Result.ShowAsync();
                        }
                        else
                        {
                            ContentDialog Result = new ContentDialog
                            {
                                Title = "结果",
                                Content = "ヾ(*ΦωΦ)ツ\n黑棋：人类胜利。",
                                CloseButtonText = "Ok"
                            };
                            ContentDialogResult result = await Result.ShowAsync();
                        }
                    }
                    else
                        PlayGame();
                }
                else
                    return;
            }
            else
            {
                bool x = invoke.Puthuman((int)Math.Round(pt.Y / deltay) - 1, (int)Math.Round(pt.X / deltax) - 1, 2);
                if (x == true)
                {
                    Clear_noticeLine();
                    DrawPoint((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, false);
                    DrawNoticeLine((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, false);
                    ++turn;
                    if (invoke.Checkwin(false) == 2)
                    {
                        WinPrinter();
                        if (!isChinese)
                        {
                            ContentDialog Result = new ContentDialog
                            {
                                Title = "Result",
                                Content = "ヾ(*ΦωΦ)ツ\nWhite: Human wins.",
                                CloseButtonText = "Ok"
                            };
                            ContentDialogResult result = await Result.ShowAsync();
                        }
                        else
                        {
                            ContentDialog Result = new ContentDialog
                            {
                                Title = "结果",
                                Content = "ヾ(*ΦωΦ)ツ\n白棋：人类胜利。",
                                CloseButtonText = "Ok"
                            };
                            ContentDialogResult result = await Result.ShowAsync();
                        }
                    }
                    else
                        PlayGame();
                }
                else
                    return;
            }
        }

        private void Page_Unloaded(object sender, RoutedEventArgs e)
        {
            operationCanceled = true;
        }

        private void UndoButton_Click(object sender, RoutedEventArgs e)
        {
            if (_gameOptions.Values["black"].ToString() == "computer" && _gameOptions.Values["white"].ToString() == "computer")
                return;
            if (_gameOptions.Values["black"].ToString() == "human" && _gameOptions.Values["white"].ToString() == "human")
            {
                if (invoke.Gethistory() != null && invoke.Gethistory().Length >= 3)
                    Clear_noticeLine();
                bool Undostatus = UndoGame();
                if (Undostatus == false)
                {
                    if (!isChinese)
                    {
                        Undo_Text.Text = "ヾ(*ΦωΦ)ツ\nNo chess to undo.";
                    }
                    else
                    {
                        Undo_Text.Text = "ヾ(*ΦωΦ)ツ\n无棋可悔。";
                    }
                    FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
                }
            }
            else
            {
                if (invoke.Gethistory() != null && invoke.Gethistory().Length >= 6)
                {
                    Clear_noticeLine();
                    UndoGame();
                    UndoGame();
                }
                else
                {
                    if (!isChinese)
                    {
                        Undo_Text.Text = "ヾ(*ΦωΦ)ツ\nNo chess to undo.";
                    }
                    else
                    {
                        Undo_Text.Text = "ヾ(*ΦωΦ)ツ\n无棋可悔。";
                    }
                    FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
                }
            }
        }

        private void RestartButton_Click(object sender, RoutedEventArgs e)
        {
            if(!isChinese)
            EvalLabel.Text = "Eval: ---";
            else
                EvalLabel.Text = "评分：---";
            Clear_noticeLine();
            while (UndoGame() == true) ;
            PlayGame();
        }

        private void DumpButton_Click(object sender, RoutedEventArgs e)
        {
            char a1, a2;
            var history = invoke.Gethistory();
            string output = "ヾ(*ΦωΦ)ツ\n";
            var length = 0;
            if (history != null)
                length = history.Length;
            if(!isChinese)
            output += "Turn: " + Convert.ToString(length / 3) + "\n";
            else
                output += "步数：" + Convert.ToString(length / 3) + "\n";
            for (int ii = 0; ii < length / 3; ++ii)
            {
                a1 = (char)(history[ii * 3 + 1] + 'A');
                a2 = (char)(history[ii * 3 + 2] + 'A');
                output += Convert.ToString(ii + 1).PadLeft(3) + ": " + Convert.ToString(a1) + Convert.ToString(a2) + " ";
                if (ii != 0 && (ii+1) % 6 == 0)
                    output += "\n";
            }
            Dump_Text.Text = output;
            FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
        }
    }
}
